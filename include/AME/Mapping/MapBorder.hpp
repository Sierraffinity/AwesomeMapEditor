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


#ifndef __AME_MAPBORDER_HPP__
#define __AME_MAPBORDER_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/System/IUndoable.hpp>
#include <AME/System/WriteEntry.hpp>
#include <AME/Mapping/MapBlock.hpp>
#include <QSize>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    MapBorder.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/11/2016
    /// \brief   Stores the border blocks around a map.
    ///
    /// FRLG and RSE differ concerning borders. RSE has a fixed
    /// border size of 2x2, whereas FRLG borders are variable
    /// in size, as long as they are in range of 255x255
    ///
    ///////////////////////////////////////////////////////////
    class MapBorder /*: IUndoable */ {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::MapBorder.
        /// Note: MapBorder is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because of the undo/redo system.
        ///
        ///////////////////////////////////////////////////////////
        MapBorder();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::MapBorder.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        MapBorder(const MapBorder &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::MapBorder
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        MapBorder &operator=(const MapBorder &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated block objects.
        ///
        ///////////////////////////////////////////////////////////
        ~MapBorder();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read the map border.
        ///
        /// Reads all map-header data, such as size, blocks and
        /// used tilesets.
        ///
        /// \param rom Currently opened ROM file
        /// \param offset Offset of the map-header
        /// \returns true if the map-header was read correctly.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset, const QSize &size);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all the border blocks.
        ///
        /// The border blocks have the same structure and share the
        /// same properties as map block entries. Their permission,
        /// though, which occupies six bits, is unused.
        ///
        /// \returns all map border blocks.
        ///
        ///////////////////////////////////////////////////////////
        const QList<MapBlock *> &blocks() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the width of the map border.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 width() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the height of the map border.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 height() const;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QList<MapBlock *> m_Blocks; ///< Holds all map border blocks
        UInt32 m_Width;             ///< Defines the border width
        UInt32 m_Height;            ///< Defines the border height
    };
}


#endif // __AME_MAPBORDER_HPP__
