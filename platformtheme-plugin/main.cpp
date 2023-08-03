#include <qpa/qplatformthemeplugin.h>
#include "lycheeplatformtheme.h"

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

class QXdgDesktopPortalThemePlugin : public QPlatformThemePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformThemeFactoryInterface_iid FILE "lycheePlatformTheme.json")

public:
    QPlatformTheme *create(const QString &key, const QStringList &params) override;
};

QPlatformTheme *QXdgDesktopPortalThemePlugin::create(const QString &key, const QStringList &params)
{
    Q_UNUSED(params);
    if (!key.compare("xdgdesktopportal"_L1, Qt::CaseInsensitive) ||
        !key.compare("flatpak"_L1, Qt::CaseInsensitive) ||
        !key.compare("snap"_L1, Qt::CaseInsensitive))
        return new LycheePlatformTheme;

    return nullptr;
}

QT_END_NAMESPACE

#include "main.moc"
