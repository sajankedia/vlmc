/*****************************************************************************
 * TracksScene.cpp: QGraphicsScene that contains the tracks
 *****************************************************************************
 * Copyright (C) 2008-2010 VideoLAN
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

#include <QMessageBox>
#include <QKeyEvent>
#include "TracksScene.h"
#include "Commands.h"
#include "GraphicsMovieItem.h"
#include "GraphicsAudioItem.h"
#include "Timeline.h"

TracksScene::TracksScene( QObject* parent ) : QGraphicsScene( parent )
{
}

void TracksScene::keyPressEvent( QKeyEvent* keyEvent )
{
    TracksView* tv = Timeline::getInstance()->tracksView();
    if ( !tv ) return;

    if ( keyEvent->modifiers() == Qt::NoModifier &&
         keyEvent->key() == Qt::Key_Delete &&
         selectedItems().size() >= 1 )
    {
        // Items deletion
        keyEvent->accept();
        askRemoveSelectedItems();
    }

    QGraphicsScene::keyPressEvent( keyEvent );
}

void TracksScene::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    QGraphicsScene::contextMenuEvent( event );
    if ( event->isAccepted() )
        return; // Event handled by an item in the scene.

    //TODO Event not handled, create and show a menu here.
}

void TracksScene::askRemoveSelectedItems()
{
    TracksView* tv = Timeline::getInstance()->tracksView();

    if ( !tv ) return;

    QString message;
    if ( selectedItems().size() == 1 )
        message = tr("Confirm the deletion of the region ?");
    else
        message = tr("Confirm the deletion of those regions ?");

    QMessageBox::StandardButton b =
    QMessageBox::warning( tv, "Object deletion",
                          message,
                          QMessageBox::Yes | QMessageBox::No,
                          QMessageBox::No );

    // Skip the deletion process
    if ( b == QMessageBox::No ) return;

    UndoStack::getInstance()->beginMacro( "Remove clip(s)" );

    QList<QGraphicsItem*> items = selectedItems();
    for (int i = 0; i < items.size(); ++i )
    {
        GraphicsMovieItem* item = qgraphicsitem_cast<GraphicsMovieItem*>( items.at(i) );
        if ( !item ) return;

        Commands::trigger( new Commands::MainWorkflow::RemoveClip( item->clip(),
                                                                   item->trackNumber(),
                                                                   item->startPos(),
                                                                   item->mediaType() ) );
    }

    UndoStack::getInstance()->endMacro();
}
