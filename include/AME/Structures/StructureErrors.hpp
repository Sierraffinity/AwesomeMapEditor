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


#ifndef __AME_STRUCTUREERRORS_HPP__
#define __AME_STRUCTUREERRORS_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/System/ErrorStack.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    StructureErrors.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/3/2016
    /// \brief   Defines all errors for 'Structure' classes.
    ///
    ///////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////
    // Class: WildPokemonTable
    //
    ///////////////////////////////////////////////////////////
    #define WPT_ERROR_OFFSET    "The location of the wildpokemon head table (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define WPT_ERROR_RANGE     "The wildpokemon head table exceeded the ROM's range."

    ///////////////////////////////////////////////////////////
    // Class: WildPokemonTable
    //
    ///////////////////////////////////////////////////////////
    #define WPST_ERROR_OFFSET   "The location of a wildpokemon sub table (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define WPST_ERROR_RANGE    "While having attempted to read the encounter data, ROM's range was exceeded."
    #define WPST_ERROR_GRASS    "The location of the grass encounters (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define WPST_ERROR_WATER    "The location of the water encounters (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define WPST_ERROR_ROCK     "The location of the rock-smash encounters (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define WPST_ERROR_FISH     "The location of the fishing encounters (ref: 0x%offset%) is invalid\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data.."
    #define WPST_ERROR_DATA     "The location of the encounter data (ref: 0x%offset%) is invalid\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data.."

    ///////////////////////////////////////////////////////////
    // Class: PokemonTable
    //
    ///////////////////////////////////////////////////////////
    #define PKM_ERROR_NAMES     "The location of the pokemon name table is invalid.\nPlease check the YAML configuration file\nfor your game and approve it's validity."
    #define PKM_ERROR_ICONS     "The location of the pokemon icon table is invalid.\nPlease check the YAML configuration file\nfor your game and approve it's validity."
    #define PKM_ERROR_USAGE     "The location of the icon palette usage table is invalid.\nPlease check the YAML configuration file\nfor your game and approve it's validity."
    #define PKM_ERROR_PALS      "The location of the icon palette table is invalid.\nPlease check the YAML configuration file\nfor your game and approve it's validity."
    #define PKM_ERROR_IMAGE     "The location of an icon image (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define PKM_ERROR_ENTRY     "The usage entry at location 0x%offset% exceeded two."
}


#endif // __AME_STRUCTUREERRORS_HPP__
