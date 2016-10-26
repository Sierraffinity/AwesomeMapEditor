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


#ifndef __AME_AMEMAPVIEW_HPP__
#define __AME_AMEMAPVIEW_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QtWidgets>
#include <QtOpenGL/QtOpenGL>
#include <AME/System/LoadedData.hpp>


namespace ame
{
    class AMEBlockView;

    ///////////////////////////////////////////////////////////
    /// \file    AMEMapView.h
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/17/2016
    /// \brief   Displays a map with all it's connections.
    ///
    ///////////////////////////////////////////////////////////
    class AMEMapView : public QOpenGLWidget, public QOpenGLFunctions {
    friend class AMEEntityView;
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes AMEMapView with a given parent.
        ///
        ///////////////////////////////////////////////////////////
        AMEMapView(QWidget *parent = NULL);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Destroys all the map images and OpenGL things.
        ///
        ///////////////////////////////////////////////////////////
        ~AMEMapView();


        ///////////////////////////////////////////////////////////
        /// \brief Sets the map to be loaded, with all connections.
        ///
        /// Attempts to load the map images and its connections. It
        /// returns false if OpenGL fails or if the map is invalid.
        ///
        /// \returns true if loading succeeded.
        ///
        ///////////////////////////////////////////////////////////
        bool setMap(const qboy::Rom &rom, Map *map);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the map layout only to be loaded
        /// \returns true if loading succeeded.
        ///
        ///////////////////////////////////////////////////////////
        bool setLayout(MapHeader &header);

        ///////////////////////////////////////////////////////////
        /// \brief Creates textures for all NPCs on the map.
        ///
        ///////////////////////////////////////////////////////////
        void setEntities(const QList<Npc *> &npcs);

        ///////////////////////////////////////////////////////////
        /// \brief Creates the textures for the images and pals.
        ///
        ///////////////////////////////////////////////////////////
        void makeGL();

        ///////////////////////////////////////////////////////////
        /// \brief Destroys all OpenGL resources.
        ///
        ///////////////////////////////////////////////////////////
        void freeGL();

        ///////////////////////////////////////////////////////////
        /// \brief Specifies whether map is in movement mode.
        ///
        ///////////////////////////////////////////////////////////
        void setMovementMode(bool isInMovementMode);

        ///////////////////////////////////////////////////////////
        /// \brief Specifies the block view.
        ///
        ///////////////////////////////////////////////////////////
        void setBlockView(AMEBlockView *view);


        ///////////////////////////////////////////////////////////
        /// \brief Determines whether being in movement mode.
        ///
        ///////////////////////////////////////////////////////////
        bool movementMode() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the primary blockset size.
        ///
        ///////////////////////////////////////////////////////////
        QPoint mainPos();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the main map.
        ///
        ///////////////////////////////////////////////////////////
        Map *mainMap();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the layout.
        ///
        ///////////////////////////////////////////////////////////
        MapHeader *layoutHeader();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the main blockset palettes.
        ///
        ///////////////////////////////////////////////////////////
        QVector<qboy::GLColor> *mainPalettes();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the blockset pixels.
        ///
        ///////////////////////////////////////////////////////////
        QList<UInt8 *> mainPixels();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the primary blockset size.
        ///
        ///////////////////////////////////////////////////////////
        QSize primarySetSize();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the secondary blockset size.
        ///
        ///////////////////////////////////////////////////////////
        QSize secondarySetSize();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the primary block count.
        ///
        ///////////////////////////////////////////////////////////
        Int32 primaryBlockCount();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the secondary block count.
        ///
        ///////////////////////////////////////////////////////////
        Int32 secondaryBlockCount();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the border.
        ///
        ///////////////////////////////////////////////////////////
        MapBorder &border();

        ///////////////////////////////////////////////////////////
        /// \brief Enumeration for the selected editing tool.
        ///
        ///////////////////////////////////////////////////////////
        enum Tool {
            None,
            Draw,
            Select,
            Fill,
            FillAll
        };

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the current editing tool.
        ///
        ///////////////////////////////////////////////////////////
        AMEMapView::Tool getCurrentTool(Qt::MouseButtons buttons);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the current editing tool from the UI.
        ///
        ///////////////////////////////////////////////////////////
        void setCurrentTool(AMEMapView::Tool tool);


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
        /// \brief Renders all the map textures.
        ///
        ///////////////////////////////////////////////////////////
        void paintGL();

        ///////////////////////////////////////////////////////////
        /// \brief Overrides the mouse press event.
        ///
        ///////////////////////////////////////////////////////////
        void mousePressEvent(QMouseEvent *event);

        ///////////////////////////////////////////////////////////
        /// \brief Overrides the mouse release event.
        ///
        ///////////////////////////////////////////////////////////
        void mouseReleaseEvent(QMouseEvent *event);

        ///////////////////////////////////////////////////////////
        /// \brief Overrides the mouse move event.
        ///
        ///////////////////////////////////////////////////////////
        void mouseMoveEvent(QMouseEvent *event);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QList<Map *> m_Maps;
        QList<UInt32> m_MapTextures;
        QList<UInt32> m_PalTextures;
        QList<QSize> m_MapSizes;
        QList<QPoint> m_MapPositions;
        QList<QPoint> m_ConnectParts;
        QList<Int32> m_MaxRows;
        QList<UInt32> m_VertexBuffers;
        QMap<UInt32, QVector<UInt32>> m_NPCTextures;
        QList<QPoint> m_NPCPositions;
        QList<UInt8> m_NPCSprites;
        UInt32 m_NPCBuffer;
        UInt32 m_IndexBuffer;
        QList<QVector<qboy::GLColor>> m_Palettes;
        QList<UInt8 *> m_BackPixelBuffers;
        QList<UInt8 *> m_ForePixelBuffers;
        QOpenGLVertexArrayObject m_VAO;
        QOpenGLShaderProgram m_Program;
        QOpenGLShaderProgram m_PmtProg;
        QOpenGLShaderProgram m_MoveProgram;
        UInt32 m_MoveTexture;
        UInt32 m_MoveBuffer;
        QSize m_PrimarySetSize;
        QSize m_SecondarySetSize;
        QSize m_WidgetSize;
        UInt8 *m_PrimaryForeground;
        UInt8 *m_PrimaryBackground;
        UInt8 *m_SecondaryForeground;
        UInt8 *m_SecondaryBackground;
        Int32 m_PrimaryBlockCount;
        Int32 m_SecondaryBlockCount;
        Boolean m_ShowSprites;
        Boolean m_LayoutView;
        MapHeader m_Header;
        Boolean m_MovementMode;
        AMEBlockView *m_BlockView;
        QVector<UInt16> m_SelectedBlocks;
        UInt16 m_FirstBlock;
        UInt16 m_LastBlock;
        UInt16 m_HighlightedBlock;
        AMEMapView::Tool m_CurrentTool;
        QColor m_CursorColor;
        Boolean m_ShowHighlight;
    };
}


#endif //__AME_AMEMAPVIEW_HPP__
