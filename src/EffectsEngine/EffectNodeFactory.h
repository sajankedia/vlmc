/*****************************************************************************
 * EffectNodeFactory.h: this class is used to instantiate a new EffectNode
 *                      which contains a plugin effect
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Vincent Carrubba <cyberbouba@gmail.com>
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

#ifndef EFFECTNODEFACTORY_H_
#define EFFECTNODEFACTORY_H_

#include <QMap>

#include "IEffectPluginCreator.h"
#include "EffectPluginTypeManager.h"
class   EffectNode;

class	EffectNodeFactory
{
 public:

  // CTOR & DTOR

    EffectNodeFactory();
    ~EffectNodeFactory();

    // SETTING FATHER

    void                        setFather( EffectNode* father );

    // EFFECT TYPES INFORMATION

    QList<QString>              getEffectNodeTypesNamesList( void ) const;
    QList<quint32>              getEffectNodeTypesIdsList( void ) const;

    QString const               getEffectNodeTypeNameByTypeId( quint32 typeId ) const;
    quint32                     getEffectNodeTypeIdByTypeName( QString const & typeName ) const;

    // EFFECT INSTANCES INFORMATIONS

    QList<QString>              getEffectNodeInstancesNamesList( void ) const;
    QList<quint32>              getEffectNodeInstancesIdsList( void ) const;

    QString const               getEffectNodeInstanceNameByInstanceId( quint32 instanceId ) const;
    quint32                     getEffectNodeInstanceIdByInstanceName( QString const & instanceName ) const;

    // CREATE AND DELETE EFFECTS

    void        createEmptyEffectNodeInstance( void );
    bool        createEmptyEffectNodeInstance( QString const & instanceName );

    bool        createEffectNodeInstance( quint32 typeId );
    bool        createEffectNodeInstance( QString const & typeName );

    bool        deleteEffectNodeInstance( quint32 instanceId );
    bool        deleteEffectNodeInstance( QString const & instanceName );

    // GETTING EFFECTS

    EffectNode* getEffectNodeInstance( quint32 instanceId ) const;
    EffectNode* getEffectNodeInstance( QString const & instanceName ) const;

    QList<EffectNode*>  getEffectNodeInstancesList( void ) const;

private:

    // NEW COOL

    QMap<quint32, EffectNode*>                  m_enById;
    QMap<QString, EffectNode*>                  m_enByName;
    QMap<quint32, QString>                      m_nameById;
    quint32                                     m_higherFreeId;
    quint32                                     m_mapHoles;

    EffectNode*                                 m_father;

    static EffectPluginTypeManager              m_eptm;

};

#include "EffectNode.h"

#endif // EFFECTNODEFACTORY_H_