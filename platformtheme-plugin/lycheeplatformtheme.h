#ifndef LYCHEEPLATFORMTHEME_H
#define LYCHEEPLATFORMTHEME_H

#include <qpa/qplatformtheme.h>

#include <QObject>

QT_BEGIN_NAMESPACE

class LycheePlatformThemePrivate;

class LycheePlatformTheme : public QPlatformTheme
{
    Q_DECLARE_PRIVATE(LycheePlatformTheme)
public:
    LycheePlatformTheme();

    QPlatformMenuItem *createPlatformMenuItem() const override;
    QPlatformMenu *createPlatformMenu() const override;
    QPlatformMenuBar *createPlatformMenuBar() const override;
    void showPlatformMenuBar() override;

    bool usePlatformNativeDialog(DialogType type) const override;
    QPlatformDialogHelper *createPlatformDialogHelper(DialogType type) const override;

#ifndef QT_NO_SYSTEMTRAYICON
    QPlatformSystemTrayIcon *createPlatformSystemTrayIcon() const override;
#endif

    const QPalette *palette(Palette type = SystemPalette) const override;
    const QFont *font(Font type = SystemFont) const override;
    QVariant themeHint(ThemeHint hint) const override;
    Qt::ColorScheme colorScheme() const override;

    QPixmap standardPixmap(StandardPixmap sp, const QSizeF &size) const override;
    QIcon fileIcon(const QFileInfo &fileInfo,
                   QPlatformTheme::IconOptions iconOptions = { }) const override;

    QIconEngine *createIconEngine(const QString &iconName) const override;

#if QT_CONFIG(shortcut)
    QList<QKeySequence> keyBindings(QKeySequence::StandardKey key) const override;
#endif

    QString standardButtonText(int button) const override;

private:
    QScopedPointer<LycheePlatformThemePrivate> d_ptr;
    Q_DISABLE_COPY_MOVE(LycheePlatformTheme)
};

QT_END_NAMESPACE

#endif // LYCHEEPLATFORMTHEME_H
