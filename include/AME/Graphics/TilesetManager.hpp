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


#ifndef __AME_TILESETMANAGER_HPP__
#define __AME_TILESETMANAGER_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Graphics/Tileset.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    TilesetManager.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/10/2016
    /// \brief   Holds all tilesets in the game.
    ///
    ///////////////////////////////////////////////////////////
    class TilesetManager {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Adds the specified tileset to the manager.
        ///
        ///////////////////////////////////////////////////////////
        static void add(Tileset *tileset);

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the specified tileset, by offset.
        ///
        /// Returns NULL if the tileset does not exist; returns the
        /// RAM pointer to tileset of the specified offset otherwise.
        ///
        ///////////////////////////////////////////////////////////
        static Tileset *get(UInt32 offset);

        ///////////////////////////////////////////////////////////
        /// \brief Frees all tilesets within the manager.
        ///
        ///////////////////////////////////////////////////////////
        static void clear();


    private:

        ///////////////////////////////////////////////////////////
        // Static class members
        //
        ///////////////////////////////////////////////////////////
        static QList<Tileset *> m_Tilesets; ///< Holds all tilesets
    };
}


#endif // __AME_TILESETMANAGER_HPP__
