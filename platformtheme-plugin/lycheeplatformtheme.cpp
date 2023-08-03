#include "lycheeplatformtheme.h"
#include "qxdgdesktopportalfiledialog_p.h"

#include <private/qguiapplication_p.h>
#include <qpa/qplatformtheme_p.h>
#include <qpa/qplatformthemefactory_p.h>
#include <qpa/qplatformintegration.h>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusReply>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

class LycheePlatformThemePrivate : public QPlatformThemePrivate
{
public:
    enum XdgColorschemePref {
        None,
        PreferDark,
        PreferLight
    };

    LycheePlatformThemePrivate()
        : QPlatformThemePrivate()
    { }

    ~LycheePlatformThemePrivate()
    {
        delete baseTheme;
    }

    /*! \internal

        Converts the given Freedesktop color scheme setting \a colorschemePref to a Qt::ColorScheme value.
        Specification: https://github.com/flatpak/xdg-desktop-portal/blob/d7a304a00697d7d608821253cd013f3b97ac0fb6/data/org.freedesktop.impl.portal.Settings.xml#L33-L45

        Unfortunately the enum numerical values are not defined identically, so we have to convert them.

        The mapping is as follows:

        Enum Index: Freedesktop definition  | Qt definition
        ----------------------------------- | -------------
        0: No preference                    | 0: Unknown
        1: Prefer dark appearance           | 2: Dark
        2: Prefer light appearance          | 1: Light
    */
    static Qt::ColorScheme colorSchemeFromXdgPref(const XdgColorschemePref colorschemePref)
    {
        switch (colorschemePref) {
        case PreferDark: return Qt::ColorScheme::Dark;
        case PreferLight: return Qt::ColorScheme::Light;
        default: return Qt::ColorScheme::Unknown;
        }
    }

    QPlatformTheme *baseTheme = nullptr;
    uint fileChooserPortalVersion = 0;
    Qt::ColorScheme colorScheme = Qt::ColorScheme::Unknown;
};

LycheePlatformTheme::LycheePlatformTheme()
    : d_ptr(new LycheePlatformThemePrivate)
{
    Q_D(LycheePlatformTheme);

    QStringList themeNames;
    themeNames += QGuiApplicationPrivate::platform_integration->themeNames();
    // 1) Look for a theme plugin.
    for (const QString &themeName : std::as_const(themeNames)) {
        d->baseTheme = QPlatformThemeFactory::create(themeName);
        if (d->baseTheme)
            break;
    }

    // 2) If no theme plugin was found ask the platform integration to
    // create a theme
    if (!d->baseTheme) {
        for (const QString &themeName : std::as_const(themeNames)) {
            d->baseTheme = QGuiApplicationPrivate::platform_integration->createPlatformTheme(themeName);
            if (d->baseTheme)
                break;
        }
        // No error message; not having a theme plugin is allowed.
    }

    // 3) Fall back on the built-in "null" platform theme.
    if (!d->baseTheme)
        d->baseTheme = new QPlatformTheme;

    // Get information about portal version
    QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.portal.Desktop"_L1,
                                                          "/org/freedesktop/portal/desktop"_L1,
                                                          "org.freedesktop.DBus.Properties"_L1,
                                                          "Get"_L1);
    message << "org.freedesktop.portal.FileChooser"_L1 << "version"_L1;
    QDBusPendingCall pendingCall = QDBusConnection::sessionBus().asyncCall(message);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, [d] (QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<QVariant> reply = *watcher;
        if (reply.isValid()) {
            d->fileChooserPortalVersion = reply.value().toUInt();
        }
        watcher->deleteLater();
    });

    // Get information about system theme preference
    message = QDBusMessage::createMethodCall("org.freedesktop.portal.Desktop"_L1,
                                             "/org/freedesktop/portal/desktop"_L1,
                                             "org.freedesktop.portal.Settings"_L1,
                                             "Read"_L1);
    message << "org.freedesktop.appearance"_L1 << "color-scheme"_L1;

    // this must not be asyncCall() because we have to set appearance now
    QDBusReply<QVariant> reply = QDBusConnection::sessionBus().call(message);
    if (reply.isValid()) {
        const QDBusVariant dbusVariant = qvariant_cast<QDBusVariant>(reply.value());
        const LycheePlatformThemePrivate::XdgColorschemePref xdgPref = static_cast<LycheePlatformThemePrivate::XdgColorschemePref>(dbusVariant.variant().toUInt());
        d->colorScheme = LycheePlatformThemePrivate::colorSchemeFromXdgPref(xdgPref);
    }
}

QPlatformMenuItem* LycheePlatformTheme::createPlatformMenuItem() const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->createPlatformMenuItem();
}

QPlatformMenu* LycheePlatformTheme::createPlatformMenu() const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->createPlatformMenu();
}

QPlatformMenuBar* LycheePlatformTheme::createPlatformMenuBar() const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->createPlatformMenuBar();
}

void LycheePlatformTheme::showPlatformMenuBar()
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->showPlatformMenuBar();
}

bool LycheePlatformTheme::usePlatformNativeDialog(DialogType type) const
{
    Q_D(const LycheePlatformTheme);

    if (type == FileDialog)
        return true;

    return d->baseTheme->usePlatformNativeDialog(type);
}

QPlatformDialogHelper* LycheePlatformTheme::createPlatformDialogHelper(DialogType type) const
{
    Q_D(const LycheePlatformTheme);

    if (type == FileDialog && d->fileChooserPortalVersion) {
        // Older versions of FileChooser portal don't support opening directories, therefore we fallback
        // to native file dialog opened inside the sandbox to open a directory.
        if (d->baseTheme->usePlatformNativeDialog(type))
            return new QXdgDesktopPortalFileDialog(static_cast<QPlatformFileDialogHelper*>(d->baseTheme->createPlatformDialogHelper(type)),
                                                   d->fileChooserPortalVersion);

        return new QXdgDesktopPortalFileDialog;
    }

    return d->baseTheme->createPlatformDialogHelper(type);
}

#ifndef QT_NO_SYSTEMTRAYICON
QPlatformSystemTrayIcon* LycheePlatformTheme::createPlatformSystemTrayIcon() const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->createPlatformSystemTrayIcon();
}
#endif

const QPalette *LycheePlatformTheme::palette(Palette type) const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->palette(type);
}

const QFont* LycheePlatformTheme::font(Font type) const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->font(type);
}

QVariant LycheePlatformTheme::themeHint(ThemeHint hint) const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->themeHint(hint);
}

Qt::ColorScheme LycheePlatformTheme::colorScheme() const
{
    Q_D(const LycheePlatformTheme);
    return d->colorScheme;
}

QPixmap LycheePlatformTheme::standardPixmap(StandardPixmap sp, const QSizeF &size) const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->standardPixmap(sp, size);
}

QIcon LycheePlatformTheme::fileIcon(const QFileInfo &fileInfo,
                                       QPlatformTheme::IconOptions iconOptions) const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->fileIcon(fileInfo, iconOptions);
}

QIconEngine * LycheePlatformTheme::createIconEngine(const QString &iconName) const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->createIconEngine(iconName);
}

#if QT_CONFIG(shortcut)
QList<QKeySequence> LycheePlatformTheme::keyBindings(QKeySequence::StandardKey key) const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->keyBindings(key);
}
#endif

QString LycheePlatformTheme::standardButtonText(int button) const
{
    Q_D(const LycheePlatformTheme);
    return d->baseTheme->standardButtonText(button);
}

QT_END_NAMESPACE
