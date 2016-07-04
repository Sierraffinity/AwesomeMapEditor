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


#ifndef __AME_MAPPINGERRORS_HPP__
#define __AME_MAPPINGERRORS_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/System/ErrorStack.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    MappingErrors.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/11/2016
    /// \brief   Defines errors for mapping structures.
    ///
    ///////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////
    // Class: MapBorder
    //
    ///////////////////////////////////////////////////////////
    #define BRD_ERROR_OFFSET    "The location of the map border (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."


    ///////////////////////////////////////////////////////////
    // Class: MapHeader
    //
    ///////////////////////////////////////////////////////////
    #define HDR_ERROR_OFFSET    "The location of the map header (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define HDR_ERROR_BORDER    "The location of the map border (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define HDR_ERROR_BLOCKS    "The location of the map blocks (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define HDR_ERROR_PRIMARY   "The location of the primary tileset (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define HDR_ERROR_SECONDARY "The location of the secondary tileset (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."

    ///////////////////////////////////////////////////////////
    // Class: Map
    //
    ///////////////////////////////////////////////////////////
    #define MAP_ERROR_OFFSET    "The location of the map (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define MAP_ERROR_HEADER    "The location of the map header (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define MAP_ERROR_EVENTS    "The location of the map events (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define MAP_ERROR_SCRIPTS   "The location of the map scripts (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define MAP_ERROR_CONNEX    "The location of the map connections (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define MAP_ERROR_NAME      "The location of the map name (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."

    ///////////////////////////////////////////////////////////
    // Class: MapBank
    //
    ///////////////////////////////////////////////////////////
    #define BNK_ERROR_OFFSET    "The location of the map bank (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define BNK_ERROR_WHILE     "Exceeded the ROM's bounds while reading the bank's maps.\nMake sure that the end of the table has an ending\nsequence that is not a pointer (e.g. 0xFF)."

    ///////////////////////////////////////////////////////////
    // Class: MapBankTable
    //
    ///////////////////////////////////////////////////////////
    #define MBT_ERROR_OFFSET    "The location of the map bank table (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define MBT_ERROR_WHILE     "Exceeded the ROM's bounds while reading the bank table.\nMake sure that the end of the table has an ending\nsequence that is not a pointer (e.g. 0xFF)."

    ///////////////////////////////////////////////////////////
    // Class: MapLayoutTable
    //
    ///////////////////////////////////////////////////////////
    #define LAY_ERROR_OFFSET    "The location of the map layout table (ref: 0x%offset%) is invalid.\nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define LAY_ERROR_WHILE     "Exceeded the ROM's bounds while reading the layout table.\nMake sure that the end of the table has an ending\nsequence that is not a pointer (e.g. 0xFF)."
}


#endif // __AME_MAPPINGERRORS_HPP__
