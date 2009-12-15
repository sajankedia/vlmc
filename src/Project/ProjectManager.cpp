/*****************************************************************************
 * ProjectManager.cpp: Manager the project loading and saving.
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

#include <QFileDialog>
#include <QtDebug>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>

#include <signal.h>

#include "ProjectManager.h"
#include "Library.h"
#include "MainWorkflow.h"
#include "SettingsManager.h"
#include "CrashHandler.h"

void    ProjectManager::signalHandler( int sig )
{
    signal( sig, SIG_DFL );

    ProjectManager::getInstance()->emergencyBackup();

    CrashHandler* ch = new CrashHandler( sig );
    ch->exec();
    QApplication::exit(1);
}

const QString   ProjectManager::unNamedProject = tr( "<Unnamed project>" );

ProjectManager::ProjectManager() : m_projectFile( NULL ), m_needSave( false )
{
    QSettings s;
    m_recentsProjects = s.value( "RecentsProjects" ).toStringList();
    connect( this, SIGNAL( projectClosed() ), Library::getInstance(), SLOT( clear() ) );
    connect( this, SIGNAL( projectClosed() ), MainWorkflow::getInstance(), SLOT( clear() ) );
    const SettingValue* val = SettingsManager::getInstance()->getValue( "project", "ProjectName");
    connect( val, SIGNAL( changed( QVariant) ), this, SLOT(nameChanged(QVariant) ) );
    m_projectName = tr( "<Unsaved project>" );
//    signal( SIGSEGV, ProjectManager::signalHandler );
//    signal( SIGINT, SIG_IGN );
    signal( SIGFPE, ProjectManager::signalHandler );
    signal( SIGABRT, ProjectManager::signalHandler );
    signal( SIGILL, ProjectManager::signalHandler );
}

ProjectManager::~ProjectManager()
{
    // Write uncommited change to the disk
    QSettings s;
    s.sync();

    if ( m_projectFile != NULL )
        delete m_projectFile;
}

bool    ProjectManager::needSave() const
{
    return m_needSave;
}

QStringList ProjectManager::recentsProjects() const
{
    return m_recentsProjects;
}

void    ProjectManager::cleanChanged( bool val )
{
    m_needSave = !val;
    if ( m_projectFile != NULL )
    {
        QFileInfo   fInfo( *m_projectFile );
    }
    emit projectUpdated( m_projectName, val );
}

void    ProjectManager::loadTimeline()
{
    QDomElement     root = m_domDocument->documentElement();
    QFileInfo       fInfo( *m_projectFile );

    MainWorkflow::getInstance()->loadProject( root.firstChildElement( "timeline" ) );
    emit projectUpdated( m_projectName, true );
}

void    ProjectManager::parseProjectNode( const QDomElement &node )
{
    QDomElement     projectNameNode = node.firstChildElement( "ProjectName" );
    m_projectName = projectNameNode.attribute( "value", ProjectManager::unNamedProject );
}

void    ProjectManager::loadProject( const QString& fileName )
{
    if ( fileName.length() == 0 )
        return;

    if ( closeProject() == false )
        return ;

    // Append the item to the recents list
    m_recentsProjects.removeAll( fileName );
    m_recentsProjects.prepend( fileName );
    while ( m_recentsProjects.count() > 15 )
        m_recentsProjects.removeLast();

    QSettings s;
    s.setValue( "RecentsProjects", m_recentsProjects );
    s.sync();

    m_projectFile = new QFile( fileName );

    m_domDocument = new QDomDocument;
    m_projectFile->open( QFile::ReadOnly );
    m_domDocument->setContent( m_projectFile );
    m_projectFile->close();

    QDomElement     root = m_domDocument->documentElement();

    parseProjectNode( root.firstChildElement( "project" ) );
    connect( Library::getInstance(), SIGNAL( projectLoaded() ), this, SLOT( loadTimeline() ) );
    Library::getInstance()->loadProject( root.firstChildElement( "medias" ) );
    SettingsManager::getInstance()->loadSettings( "project", root.firstChildElement( "project" ) );
}

QString  ProjectManager::loadProjectFile()
{
    QString fileName =
            QFileDialog::getOpenFileName( NULL, "Enter the output file name",
                                          QString(), "VLMC project file(*.vlmc)" );
    return fileName;
}

bool    ProjectManager::checkProjectOpen( bool saveAs )
{
    if ( m_projectFile == NULL || saveAs == true )
    {
        QString outputFileName =
            QFileDialog::getSaveFileName( NULL, "Enter the output file name",
                                          QString(), "VLMC project file(*.vlmc)" );
        if ( outputFileName.length() == 0 )
            return false;
        if ( m_projectFile != NULL )
            delete m_projectFile;
        if ( outputFileName.endsWith( ".vlmc" ) == false )
            outputFileName += ".vlmc";
        m_projectFile = new QFile( outputFileName );
    }
    return true;
}

void    ProjectManager::saveProject( bool saveAs /*= true*/ )
{
    if ( checkProjectOpen( saveAs ) == false )
        return ;
    __saveProject( m_projectFile->fileName() );
    if ( saveAs == true )
    {
        QFileInfo   fInfo( *m_projectFile );
        emit projectUpdated( fInfo.fileName(), true );
    }
    emit projectSaved();
}

void    ProjectManager::__saveProject( const QString &fileName )
{
    QDomImplementation    implem = QDomDocument().implementation();
    //FIXME: Think about naming the project...
    QString name = "VLMCProject";
    QString publicId = "-//XADECK//DTD Stone 1.0 //EN";
    QString systemId = "http://www-imagis.imag.fr/DTD/stone1.dtd";
    QDomDocument doc(implem.createDocumentType( name, publicId, systemId ) );

    QDomElement rootNode = doc.createElement( "vlmc" );

    Library::getInstance()->saveProject( doc, rootNode );
    MainWorkflow::getInstance()->saveProject( doc, rootNode );
    SettingsManager::getInstance()->saveSettings( "project", doc, rootNode );
    SettingsManager::getInstance()->saveSettings( "keyboard_shortcut", doc, rootNode );

    doc.appendChild( rootNode );

    QFile   file( fileName );
    file.open( QFile::WriteOnly );
    file.write( doc.toString().toAscii() );
    file.close();
}

void    ProjectManager::newProject( const QString &projectName )
{
    if ( closeProject() == false )
        return ;
    m_projectName = projectName;
    emit projectUpdated( m_projectName, true );
}

bool    ProjectManager::closeProject()
{
    if ( askForSaveIfModified() == false )
        return false;
    if ( m_projectFile != NULL )
    {
        delete m_projectFile;
        m_projectFile = NULL;
    }
    emit projectClosed();
    return true;
}

bool    ProjectManager::askForSaveIfModified()
{
    if ( m_needSave == true )
    {
        QMessageBox msgBox;
        msgBox.setText( tr( "The project has been modified." ) );
        msgBox.setInformativeText( tr( "Do you want to save it ?" ) );
        msgBox.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        msgBox.setDefaultButton(QMessageBox::Save);
        int     ret = msgBox.exec();

        switch ( ret )
        {
            case QMessageBox::Save:
                saveProject();
                break ;
            case QMessageBox::Discard:
                break ;
            case QMessageBox::Cancel:
            default:
                return false ;
        }
    }
    return true;
}

void    ProjectManager::nameChanged( const QVariant& name )
{
    m_projectName = name.toString();
}

void    ProjectManager::emergencyBackup()
{
    if ( m_projectFile != NULL )
    {
        QString name = m_projectFile->fileName();
        name += "backup";
        __saveProject( name );
    }
    else
    {
        QString name = QDir::currentPath() + "unsavedproject.vlmcbackup";
        __saveProject( name );
    }
}
