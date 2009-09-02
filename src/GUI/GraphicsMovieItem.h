/*****************************************************************************
 * GraphicsMovieItem.h: Represent a movie graphically in the timeline
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Ludovic Fauvet <etix@l0cal.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef GRAPHICSMOVIEITEM_H
#define GRAPHICSMOVIEITEM_H

#include <QFontMetrics>
#include "AbstractGraphicsMediaItem.h"
#include "Clip.h"
#include "TracksView.h"

class GraphicsMovieItem : public AbstractGraphicsMediaItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 1 };
    GraphicsMovieItem( Clip* clip );
    virtual ~GraphicsMovieItem();

    virtual int type() const { return Type; }
    virtual bool expandable() const { return false; }
    virtual bool moveable() const { return true; }
    virtual const QUuid& uuid() const { return m_clip->getTimelineUuid(); }
    virtual QRectF boundingRect() const;
    virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );

    void setWidth( int width );
    void setHeight( int height );
    Clip* clip() const;
    void    setClip( Clip* clip );
    //void setAudioSpectrum( QVector<float*> spectrum );
    void paintAudioSpectrum( QPainter* painter );

private:
    void                updateTitle();
    Clip*               m_clip;
    int                 m_width;
    int                 m_height;
    QVector<float*>     m_audioSpectrum;
    QGraphicsTextItem*  m_movieTitle;
};

#endif // GRAPHICSMOVIEITEM_H
