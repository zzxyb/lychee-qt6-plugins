#ifndef LYCHEEDECORATION_H
#define LYCHEEDECORATION_H

#include <QObject>
#include <QScopedPointer>

#include <QtWaylandClient/private/qwaylanddecorationplugin_p.h>
#include <QtWaylandClient/private/qwaylandabstractdecoration_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

#define BUTTON_SPACING 5
#define BUTTON_WIDTH 18
#define BUTTONS_RIGHT_MARGIN 8

enum Button
{
    None,
    Close,
    Maximize,
    Minimize
};

class LycheeDecorationPrivate;

class Q_WAYLANDCLIENT_EXPORT LycheeDecoration : public QWaylandAbstractDecoration
{
    Q_OBJECT
public:
    LycheeDecoration();
    ~LycheeDecoration() override;

protected:
    QMargins margins(MarginsType marginsType = Full) const override;
    void paint(QPaintDevice *device) override;
    bool handleMouse(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,Qt::MouseButtons b,Qt::KeyboardModifiers mods) override;
    bool handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global, QEventPoint::State state, Qt::KeyboardModifiers mods) override;
private:
    enum class PointerType {
        Mouse,
        Touch
    };

    void processPointerTop(QWaylandInputDevice *inputDevice, const QPointF &local, Qt::MouseButtons b,Qt::KeyboardModifiers mods, PointerType type);
    void processPointerBottom(QWaylandInputDevice *inputDevice, const QPointF &local, Qt::MouseButtons b,Qt::KeyboardModifiers mods, PointerType type);
    void processPointerLeft(QWaylandInputDevice *inputDevice, const QPointF &local, Qt::MouseButtons b,Qt::KeyboardModifiers mods, PointerType type);
    void processPointerRight(QWaylandInputDevice *inputDevice, const QPointF &local, Qt::MouseButtons b,Qt::KeyboardModifiers mods, PointerType type);
    bool clickButton(Qt::MouseButtons b, Button btn);

    QRectF closeButtonRect() const;
    QRectF maximizeButtonRect() const;
    QRectF minimizeButtonRect() const;

    QScopedPointer<LycheeDecorationPrivate> d;

    Q_DISABLE_COPY(LycheeDecoration)
};
}

QT_END_NAMESPACE

#endif // LYCHEEDECORATION_H
