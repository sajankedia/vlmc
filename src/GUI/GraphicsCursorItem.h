#ifndef GRAPHICSCURSORITEM_H
#define GRAPHICSCURSORITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include <QRectF>
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

class GraphicsCursorItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    GraphicsCursorItem( int height, const QPen& pen );
    int cursorPos() const { return ( int )pos().x(); }
    virtual QRectF boundingRect() const;
    void setHeight( int height );

protected:
    virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
    virtual QVariant itemChange( GraphicsItemChange change, const QVariant& value );

private:
    QPen m_pen;
    QRectF m_boundingRect;

signals:
    void cursorPositionChanged( qint64 pos );

public slots:
    void setCursorPos( qint64 position );
};

#endif // GRAPHICSCURSORITEM_H
