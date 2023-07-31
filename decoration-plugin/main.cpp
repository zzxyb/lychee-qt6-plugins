#include "lycheedecoration.h"

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

class QWaylandBradientDecorationPlugin : public QWaylandDecorationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandDecorationFactoryInterface_iid FILE "lycheeDecoration.json")
public:
    QWaylandAbstractDecoration *create(const QString&, const QStringList&) override;
};

QWaylandAbstractDecoration *QWaylandBradientDecorationPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);
    Q_UNUSED(system);
    return new LycheeDecoration();
}

}

QT_END_NAMESPACE

#include "main.moc"
