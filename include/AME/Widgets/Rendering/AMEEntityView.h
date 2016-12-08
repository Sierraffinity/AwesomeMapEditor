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
        int index = 0;
    };


    ///////////////////////////////////////////////////////////
    /// \file    AMEEntityView.h
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/19/2016
    /// \brief   Displays a map with all it's entities.
    ///
    ///////////////////////////////////////////////////////////
    class AMEEntityView : public QWidget {
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
        /// \brief Specifies the currently selected entity.
        ///
        ///////////////////////////////////////////////////////////
        void setOverworlds(const QList<QImage> *ows);

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


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief
        ///
        ///////////////////////////////////////////////////////////
        void paintEvent(QPaintEvent *event);

        ///////////////////////////////////////////////////////////
        /// \brief
        ///
        ///////////////////////////////////////////////////////////
        void mousePressEvent(QMouseEvent *event);

        ///////////////////////////////////////////////////////////
        /// \brief Overrides the mouse double-click event.
        ///
        ///////////////////////////////////////////////////////////
        void mouseDoubleClickEvent(QMouseEvent *event);


    signals:

        void onMouseClick(QMouseEvent *event);
        void onDoubleClick(QMouseEvent *event);
        bool loadMapChangeTreeView(Map *map);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        EventTable *m_Entities;         ///< Holds all entities
        AMEMapView *m_MapView;          ///< Abuses the map view
        QPoint m_Translation;           ///< Fix for connections
        QImage m_FieldImage;            ///< P, S, S, W sign image
        CurrentEntity m_Selection;      ///< Currently selected entity
        Boolean m_IsInit;
        const QList<QImage> *m_OW;      ///< Overworld images
    };
}


#endif //__AME_AMEENTITYVIEW_HPP__
