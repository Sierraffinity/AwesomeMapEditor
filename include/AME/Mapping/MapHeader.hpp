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


#ifndef __AME_MAPHEADER_HPP__
#define __AME_MAPHEADER_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/System/IUndoable.hpp>
#include <AME/System/WriteEntry.hpp>
#include <AME/Graphics/Tileset.hpp>
#include <AME/Mapping/MapBlock.hpp>
#include <AME/Mapping/MapBorder.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    MapHeader.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/11/2016
    /// \brief   Defines an image containing several tiles.
    ///
    /// Undo/redo system to be implemented later.
    ///
    ///////////////////////////////////////////////////////////
    class MapHeader /*: public IUndoable*/ {
    friend class MainWindow;
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::MapHeader.
        /// Note: MapHeader is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because of the undo/redo system.
        ///
        ///////////////////////////////////////////////////////////
        MapHeader();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::MapHeader.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        MapHeader(const MapHeader &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::MapHeader
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        MapHeader &operator=(const MapHeader &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated block objects.
        ///
        ///////////////////////////////////////////////////////////
        ~MapHeader();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read all map-header properties.
        ///
        /// Reads all map-header data, such as size, blocks and
        /// used tilesets.
        ///
        /// \param rom Currently opened ROM file
        /// \param offset Offset of the map-header
        /// \returns true if the map-header was read correctly.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the border for this map.
        ///
        ///////////////////////////////////////////////////////////
        MapBorder &border();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all the map-blocks.
        ///
        ///////////////////////////////////////////////////////////
        const QList<MapBlock *> &blocks() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the primary tileset for this map.
        ///
        ///////////////////////////////////////////////////////////
        Tileset *primary() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the secondary tileset for this map.
        ///
        ///////////////////////////////////////////////////////////
        Tileset *secondary() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the block size of the map (size/16).
        ///
        ///////////////////////////////////////////////////////////
        const QSize size() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the raw data of this structure.
        ///
        ///////////////////////////////////////////////////////////
        QByteArray rawData();


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt32 m_Offset;              ///< Offset of the MapHeader
        UInt32 m_Width;               ///< Width of the map in blocks
        UInt32 m_Height;              ///< Height of the map in blocks
        UInt32 m_PtrBlocks;           ///< Offset of the block data
        UInt32 m_PtrBorder;           ///< Offset of the map border
        UInt32 m_PtrPrimary;          ///< Offset of the primary tileset
        UInt32 m_PtrSecondary;        ///< Offset of the secondary tileset
        QList<MapBlock *> m_Blocks;   ///< Holds the actual map blocks
        MapBorder m_Border;           ///< Holds the map border blocks
        Tileset *m_Primary;           ///< RAM location of primary set
        Tileset *m_Secondary;         ///< RAM location of secondary set
    };
}


#endif // __AME_MAPHEADER_HPP__
