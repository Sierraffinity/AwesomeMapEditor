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
#include <QVector>
#include <QRgb>
#include <AME/System/LoadedData.hpp>
#include <AME/Widgets/Listeners/MovePermissionListener.h>
#include <AME/Widgets/Rendering/Cursor.hpp>
#include <AME/Mapping/CurrentMapManager.hpp>


namespace ame
{
    class AMEBlockView;

    ///////////////////////////////////////////////////////////
    /// \file    AMEMapView.h
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/17/2016
    /// \brief   Displays a map with all its connections.
    ///
    ///////////////////////////////////////////////////////////
    class AMEMapView : public QWidget {
    friend class AMEEntityView;
    friend class AMEBorderView;
    friend class AMEBlockView;
    Q_OBJECT
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
        const QVector<QRgb> &mainPalettes() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the blockset foreground image.
        ///
        ///////////////////////////////////////////////////////////
        const QImage &blockForeground() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the blockset background image.
        ///
        ///////////////////////////////////////////////////////////
        const QImage &blockBackground() const;

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
        Cursor::Tool getCurrentTool(Qt::MouseButtons buttons);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the current editing tool from the UI.
        ///
        ///////////////////////////////////////////////////////////
        void setCurrentTool(AMEMapView::Tool tool);

        ///////////////////////////////////////////////////////////
        /// \brief Action that occurs when draw tool used.
        ///
        ///////////////////////////////////////////////////////////
        void drawOnMousePress(QPoint pos, QVector<MapBlock> selected, int selectionWidth, int selectionHeight);

        ///////////////////////////////////////////////////////////
        /// \brief Changes a block in both the map data and image.
        ///
        ///////////////////////////////////////////////////////////
        bool placeBlock(int x, int y, MapBlock newBlock);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the visibility of the grid from the UI.
        ///
        ///////////////////////////////////////////////////////////
        void setGridVisible(bool visible);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the associated permission editor object.
        ///
        ///////////////////////////////////////////////////////////
        void setMPListener(MovePermissionListener *listener);

		///////////////////////////////////////////////////////////
		/// \brief Sets the associated map block manager object.
		///
		///////////////////////////////////////////////////////////
		void setBlockManager(CurrentMapManager *manager);

    protected:

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

        ///////////////////////////////////////////////////////////
        /// \brief Overrides the mouse double-click event.
        ///
        ///////////////////////////////////////////////////////////
        void mouseDoubleClickEvent(QMouseEvent *event);

        ///////////////////////////////////////////////////////////
        /// \brief Overrides the mouse leave event.
        ///
        ///////////////////////////////////////////////////////////
        void leaveEvent(QEvent *event);

        ///////////////////////////////////////////////////////////
        /// \brief Paints the map image.
        ///
        ///////////////////////////////////////////////////////////
        void paintEvent(QPaintEvent *event);

    signals:
        bool loadMapChangeTreeView(Map *map);

    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QList<Map *> m_Maps;
        QList<QSize> m_MapSizes;
        QList<QPoint> m_MapPositions;
        QList<QPoint> m_ConnectParts;
        QList<Int32> m_MaxRows;
        QList<QPoint> m_NPCPositions;
        QList<UInt8> m_NPCSprites;
        QVector<QRgb> m_Palettes;
        QVector<QRgb> m_PalCopy;
        QVector<QVector<QRgb>> m_ConnPalettes;
        QImage m_BlockForeground;
        QImage m_BlockBackground;
        QImage m_iMapForeground;
        QImage m_iMapBackground;
        QPixmap m_MapForeground;
        QPixmap m_MapBackground;
        QList<QImage> m_ConnMapFore;
        QList<QImage> m_ConnMapBack;
        QList<QImage> m_ConnImages;
        QList<UInt8 *> m_BackPixelBuffers;
        QList<UInt8 *> m_ForePixelBuffers;
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
        MovePermissionListener *m_MPListener;
        QVector<MapBlock> m_SelectedBlocks;
        QPoint m_FirstBlock;
        QPoint m_LastBlock;
        QPoint m_HighlightedBlock;
        QSize m_SelectSize;
        Cursor::Tool m_CurrentTool;
        QImage m_MovePerm;
        Boolean m_ShowCursor;
        Boolean m_ShowGrid;
        Boolean m_IsInit;
        Boolean m_ValidPress;
        Int32 m_HoveredConnection;
		Cursor m_Cursor;
        CurrentMapManager m_CurrentMap;
    };
}


#endif //__AME_AMEMAPVIEW_HPP__
