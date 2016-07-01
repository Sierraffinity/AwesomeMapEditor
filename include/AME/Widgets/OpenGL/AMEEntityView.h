//////////////////////////////////////////////////////////////////////////////////
//
//
//                     d88b         888b           d888  888888888888
//                    d8888b        8888b         d8888  888
//                   d88''88b       888'8b       d8'888  888
//                  d88'  '88b      888 '8b     d8' 888  8888888
//                 d88Y8888Y88b     888  '8b   d8'  888  888
//                d88""""""""88b    888   '8b d8'   888  888
//               d88'        '88b   888    '888'    888  888
//              d88'          '88b  888     '8'     888  888888888888
//
//
// AwesomeMapEditor: A map editor for GBA Pokémon games.
// Copyright (C) 2016 Diegoisawesome, Pokedude
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//////////////////////////////////////////////////////////////////////////////////


#ifndef __AME_AMEENTITYVIEW_HPP__
#define __AME_AMEENTITYVIEW_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Widgets/OpenGL/AMEMapView.h>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \brief Holds the currently selected entity.
    ///
    ///////////////////////////////////////////////////////////
    struct CurrentEntity
    {
        QPoint absPos;
        EntityType type = ET_Invalid;
        void *entity = NULL;
    };


    ///////////////////////////////////////////////////////////
    /// \file    AMEEntityView.h
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/19/2016
    /// \brief   Displays a map with all it's entities.
    ///
    ///////////////////////////////////////////////////////////
    class AMEEntityView : public QOpenGLWidget, public QOpenGLFunctions {
    Q_OBJECT
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes AMEEntityView with a given parent.
        ///
        ///////////////////////////////////////////////////////////
        AMEEntityView(QWidget *parent = NULL);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Destroys all the map images and OpenGL things.
        ///
        ///////////////////////////////////////////////////////////
        ~AMEEntityView();


        ///////////////////////////////////////////////////////////
        /// \brief Sets the entity data for this widget.
        ///
        ///////////////////////////////////////////////////////////
        void setEntities(Map *map);

        ///////////////////////////////////////////////////////////
        /// \brief Creates the textures for the images and pals.
        ///
        ///////////////////////////////////////////////////////////
        void setMapView(AMEMapView *view);

        ///////////////////////////////////////////////////////////
        /// \brief Specifies the currently selected entity.
        ///
        ///////////////////////////////////////////////////////////
        void setCurrentEntity(CurrentEntity entity);

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the currently selected entity.
        ///
        ///////////////////////////////////////////////////////////
        const CurrentEntity &currentEntity() const;

        ///////////////////////////////////////////////////////////
        /// \brief Destroys all OpenGL resources.
        ///
        ///////////////////////////////////////////////////////////
        void freeGL();


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Initializes OpenGL-related things.
        ///
        ///////////////////////////////////////////////////////////
        void initializeGL();

        ///////////////////////////////////////////////////////////
        /// \brief Resizes the viewport and redraws the objects.
        ///
        ///////////////////////////////////////////////////////////
        void resizeGL(int w, int h);

        ///////////////////////////////////////////////////////////
        /// \brief Renders all the map textures and event images.
        ///
        ///////////////////////////////////////////////////////////
        void paintGL();

        ///////////////////////////////////////////////////////////
        /// \brief
        ///
        ///////////////////////////////////////////////////////////
        void mousePressEvent(QMouseEvent *event);


    signals:

        void onMouseClick(QMouseEvent *event);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        EventTable *m_Entities;         ///< Holds all entities
        AMEMapView *m_MapView;          ///< Abuses the map view
        QOpenGLVertexArrayObject m_VAO; ///< VAO for entities
        QOpenGLShaderProgram m_Program; ///< 32ARGB textures
        QOpenGLShaderProgram m_PmtProg; ///< Primitive program
        UInt32 m_TextureAtlas;          ///< Multiple textures
        UInt32 m_VertexBuffer;
        UInt32 m_IndexBuffer;
        QPoint m_Translation;           ///< Fix for connections
        CurrentEntity m_Selection;      ///< Currently selected entity
    };
}


#endif //__AME_AMEENTITYVIEW_HPP__
