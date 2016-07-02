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


#ifndef __AME_WARP_HPP__
#define __AME_WARP_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    Warp.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/4/2016
    /// \brief   Defines a triggerable warp-field.
    ///
    ///////////////////////////////////////////////////////////
    struct Warp
    {
        UInt16 positionX;   ///< X-position on the map
        UInt16 positionY;   ///< Y-position on the map
        UInt8 level;        ///< Level/layer/height on the map
        UInt8 warp;         ///< Destination warp number
        UInt8 bank;         ///< Destination bank
        UInt8 map;          ///< Destination map
        UInt32 offset;      ///< Offset of this entity
        QByteArray rawData();
    };
}


#endif // __AME_WARP_HPP__
