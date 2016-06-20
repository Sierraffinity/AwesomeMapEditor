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


#ifndef __AME_LOADEDDATA_HPP__
#define __AME_LOADEDDATA_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/Structures/WildPokemonTable.hpp>
#include <AME/Structures/PokemonTable.hpp>
#include <AME/Graphics/TilesetManager.hpp>
#include <AME/Graphics/OverworldTable.hpp>
#include <AME/Mapping/MapBankTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    LoadedData.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/16/2016
    /// \brief   Holds all loaded map-related data.
    ///
    ///////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    /// \brief Loads all map-related data from the given ROM.
    /// \returns true if no errors occured.
    ///
    ///////////////////////////////////////////////////////////
    extern bool loadAllMapData(const qboy::Rom &rom);

    ///////////////////////////////////////////////////////////
    /// \brief Clears all loaded map-related data.
    ///
    ///////////////////////////////////////////////////////////
    extern void clearAllMapData();


    ///////////////////////////////////////////////////////////
    // Global objects
    //
    ///////////////////////////////////////////////////////////
    extern WildPokemonTable *dat_WildPokemonTable;
    extern MapBankTable *dat_MapBankTable;
    extern OverworldTable *dat_OverworldTable;
    extern PokemonTable *dat_PokemonTable;


    ///////////////////////////////////////////////////////////
    // Error messages
    //
    ///////////////////////////////////////////////////////////
}


#endif // __AME_LOADEDDATA_HPP__
