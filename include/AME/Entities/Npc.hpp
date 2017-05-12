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


#ifndef __AME_NPC_HPP__
#define __AME_NPC_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <QByteArray>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    Npc.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/4/2016
    /// \brief   Defines a non-playable character on the map.
    ///
    ///////////////////////////////////////////////////////////
    struct Npc
    {
        UInt8 npcID;        ///< ID of the person on the map
        UInt8 imageID;      ///< ID of the overworld image
        UInt8 replacement;  ///< Replacement index
        UInt8 filler_1;     ///< First filler byte
        UInt16 positionX;   ///< X-position on the map
        UInt16 positionY;   ///< Y-position on the map
        UInt8 level;        ///< Level/layer/height on the map
        UInt8 behaviour;    ///< Walking behaviour on the map
        UInt8 moveRadiusX;  ///< Walking radius, vertical
        UInt8 moveRadiusY;  ///< Walking radius, horizontal
        UInt8 filler_2;     ///< Second filler byte
        UInt8 isTrainer;    ///< If npc is trainer = 1
        UInt8 filler_3;     ///< Third filler byte
        UInt16 viewRadius;  ///< View radius for trainers
        UInt32 ptrScript;   ///< Offset of the script
        UInt16 flag;        ///< Visibility flag (set = invisible)
        UInt32 offset;      ///< Offset of this entity
        UInt8 filler_4;     ///< Fourth filler byte
        UInt8 filler_5;     ///< Fifth filler byte
        QByteArray rawData();
    };
}


#endif // __AME_NPC_HPP__
