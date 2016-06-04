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
// Copyright (C) 2016 diegoisawesome, Pokedude
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


#ifndef __AME_ENTITYTYPES_HPP__
#define __AME_ENTITYTYPES_HPP__


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    EntityTypes.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/4/2016
    /// \brief   Defines types for different entities.
    ///
    ///////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////
    // Struct: Sign
    //
    ///////////////////////////////////////////////////////////
    enum SignType : char
    {
        ST_Invalid      = -1,
        ST_Script       =  0,
        ST_ScriptUp     =  1,
        ST_ScriptDown   =  2,
        ST_ScriptRight  =  3,
        ST_ScriptLeft   =  4,
        ST_HiddenItem0  =  5,
        ST_HiddenItem1  =  6,
        ST_HiddenItem2  =  7,
        ST_SecretBase   =  8
    };

    ///////////////////////////////////////////////////////////
    // Struct: MapScript
    //
    ///////////////////////////////////////////////////////////
    enum MapScriptType : char
    {
        MST_Invalid     = -1,
        MST_Terminate   =  0,
        MST_Setmaptile  =  1,
        MST_HandlerEB0  =  2,
        MST_OnMapEnter  =  3,
        MST_HandlerF28  =  4,
        MST_CloseMenu1  =  5,
        MST_Unknown     =  6,
        MST_CloseMenu2  =  7
    };

    ///////////////////////////////////////////////////////////
    // Struct: Connection
    //
    ///////////////////////////////////////////////////////////
    enum DirectionType : char
    {
        DIR_Invalid     = -1,
        DIR_None        =  0,
        DIR_Down        =  1,
        DIR_Up          =  2,
        DIR_Left        =  3,
        DIR_Right       =  4,
        DIR_Dive        =  5,
        DIR_Emerge      =  6
    };
}


#endif // __AME_ENTITYTYPES_HPP__
