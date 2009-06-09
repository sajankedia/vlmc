#include <QtDebug>
#include "GraphicsCursorItem.h"

GraphicsCursorItem::GraphicsCursorItem( int height, const QPen& pen )
        : m_height( height ), m_pen( pen )
{
    setFlags( QGraphicsItem::ItemIgnoresTransformations | QGraphicsItem::ItemIsMovable );
    setCursor( QCursor( Qt::SizeHorCursor ) );
    setZValue( 100 );

    m_boundingRect = QRectF( -2, 0, 3, m_height );
}

QRectF GraphicsCursorItem::boundingRect() const
{
    return m_boundingRect;
}

void GraphicsCursorItem::paint( QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* )
{
    painter->setPen( m_pen );
    painter->drawLine( 0, 0, 0, m_height );
}

QVariant GraphicsCursorItem::itemChange( GraphicsItemChange change, const QVariant& value )
{
    if ( change == ItemPositionChange )
    {
        qreal posX = value.toPointF().x();
        if ( posX < 0 ) posX = 0;
        return QPoint( posX, pos().y() );
    }
    if ( change == ItemPositionHasChanged )
    {
        emit cursorPositionChanged( pos().x() );
    }
    return QGraphicsItem::itemChange( change, value );
}

void GraphicsCursorItem::setCursorPos( int position )
{
    setPos( position, pos().y() );
}

void    GraphicsCursorItem::updateCursorPos( qint64 position )
{
    setCursorPos( (qint64) position );
}
