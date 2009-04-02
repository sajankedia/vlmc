/*****************************************************************************
 * VLCMediaPlayer.cpp: Binding for libvlc_media_player
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Hugo Beauzee-Luyssen <hugo@vlmc.org>
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

#include <QtDebug>
#include <cassert>
#include "VLCMediaPlayer.h"

using namespace LibVLCpp;

MediaPlayer::MediaPlayer( Media* media, bool playStop /* = true*/ )
{
    m_internalPtr = libvlc_media_player_new_from_media( media->getInternalPtr(), m_ex );
    m_ex.checkThrow();
}

void                            MediaPlayer::play()
{
    libvlc_media_player_play( m_internalPtr, m_ex );
    m_ex.checkThrow();
}

void                            MediaPlayer::pause()
{
    libvlc_media_player_pause( m_internalPtr, m_ex );
    m_ex.checkThrow();
}

void                            MediaPlayer::stop()
{
    libvlc_media_player_stop( m_internalPtr, m_ex );
    m_ex.checkThrow();
}

qint64                          MediaPlayer::getTime()
{
    qint64 t = libvlc_media_player_get_time( m_internalPtr, m_ex );
    m_ex.checkThrow();
    return t;
}

void                            MediaPlayer::setTime( qint64 time )
{
//    qDebug() << "Setting media time to " << time;
    libvlc_media_player_set_time( m_internalPtr, time, m_ex );
    m_ex.checkThrow();
}

qint64                          MediaPlayer::getLength()
{
    qint64 length = libvlc_media_player_get_length( m_internalPtr, m_ex );
    m_ex.checkThrow();
    //qDebug() << "Media length: " << length;
    return length;
}

void                            MediaPlayer::takeSnapshot( char* outputFile, unsigned int width, unsigned int heigth )
{
    libvlc_video_take_snapshot( *this, outputFile, width, heigth, m_ex);
    m_ex.checkThrow();
}

bool                                MediaPlayer::isPlaying()
{
    int res = libvlc_media_player_is_playing( m_internalPtr, m_ex );
    m_ex.checkThrow();
    return (res == 1);
}

bool                                MediaPlayer::isSeekable()
{
    int res = libvlc_media_player_is_seekable( m_internalPtr, m_ex );
    m_ex.checkThrow();
    return (res == 1);
}

void                                MediaPlayer::setDrawable(int handle)
{
    libvlc_drawable_t   window = handle;
    libvlc_media_player_set_drawable( m_internalPtr, window, m_ex );
}
