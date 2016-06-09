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
    /// \file    EntityErrors.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/4/2016
    /// \brief   Defines all errors for 'Entity' classes.
    ///
    ///////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////
    // Class: EntityTable
    //
    ///////////////////////////////////////////////////////////
    #define EVT_ERROR_OFFSET    "The location of the event table (ref: 0x%offset%) is invalid."
    #define EVT_ERROR_SUBEVT    "The location of a sub event (ref: 0x%offset%) is invalid."
    #define EVT_ERROR_SCRIPT    "The location of a script (ref: 0x%offset%) is invalid."
    #define EVT_ERROR_TYPE      "The type of the sign-post (ref: 0x%offset%) is invalid."

    ///////////////////////////////////////////////////////////
    // Class: ConnectionTable
    //
    ///////////////////////////////////////////////////////////
    #define COT_ERROR_OFFSET    "The location of the connection table (ref: 0x%offset%) is invalid."
    #define COT_ERROR_OFFDAT    "The location of the connection data (ref: 0x%offset%) is invalid."
    #define COT_ERROR_DIRVAL    "The direction value for a connection (ref: 0x%offset%) exceeded six."

    ///////////////////////////////////////////////////////////
    // Class: MapScriptTable
    //
    ///////////////////////////////////////////////////////////
    #define MST_ERROR_OFFSET    "The location of the mapscript table (ref: 0x%offset%) is invalid."
    #define MST_ERROR_SCRIPT    "The location of a map script (ref: 0x%offset%) is invalid."
    #define MST_ERROR_WHILE     "During the process of reading the scripts, the ROM's range was exceeded."
    #define MST_ERROR_STYPE     "The script type (ref: 0x%offset%) exceeded 7."
}


#endif // __AME_STRUCTUREERRORS_HPP__
