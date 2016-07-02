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


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Entities/Npc.hpp>
#include <AME/Entities/Warp.hpp>
#include <AME/Entities/Sign.hpp>
#include <AME/Entities/Trigger.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    QByteArray Npc::rawData()
    {
        QByteArray ba;
        ba.push_back((Int8) npcID);
        ba.push_back((Int8) imageID);
        ba.push_back((Int8) replacement);
        ba.push_back((Int8) filler_1);
        ba.append((Int8 *) &positionX, 2);
        ba.append((Int8 *) &positionY, 2);
        ba.push_back((Int8) level);
        ba.push_back((Int8) behaviour);
        ba.push_back((Int8) ((moveRadiusY << 4) | moveRadiusX));
        ba.push_back((Int8) filler_2);
        ba.push_back((Int8) property);
        ba.push_back((Int8) filler_3);
        ba.append((Int8 *) &viewRadius, 2);
        ptrScript += 0x08000000;
        ba.append((Int8 *) &ptrScript, 4);
        ptrScript -= 0x08000000;
        ba.append((Int8 *) &flag, 2);
        ba.push_back((Int8) filler_4);
        ba.push_back((Int8) filler_5);

        return ba;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    QByteArray Warp::rawData()
    {
        QByteArray ba;
        ba.append((Int8 *) &positionX, 2);
        ba.append((Int8 *) &positionY, 2);
        ba.push_back((Int8) level);
        ba.push_back((Int8) warp);
        ba.push_back((Int8) bank);
        ba.push_back((Int8) map);

        return ba;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    QByteArray Trigger::rawData()
    {
        QByteArray ba;
        ba.append((Int8 *) &positionX, 2);
        ba.append((Int8 *) &positionY, 2);
        ba.push_back((Int8) level);
        ba.push_back((Int8) filler_1);
        ba.append((Int8 *) &variable, 2);
        ba.append((Int8 *) &value, 2);
        ba.push_back((Int8) filler_2);
        ba.push_back((Int8) filler_3);
        ptrScript += 0x08000000;
        ba.append((Int8 *) &ptrScript, 4);
        ptrScript -= 0x08000000;

        return ba;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    QByteArray Sign::rawData()
    {
        QByteArray ba;
        ba.append((Int8 *) &positionX, 2);
        ba.append((Int8 *) &positionY, 2);
        ba.push_back((Int8) level);
        ba.push_back((Int8) type);
        ba.push_back((Int8) filler_1);
        ba.push_back((Int8) filler_2);

        if (type <= ST_ScriptLeft)
        {
            ptrScript += 0x08000000;
            ba.append((Int8 *) &ptrScript, 4);
            ptrScript -= 0x08000000;
        }
        else if (type >= ST_HiddenItem0 && type <= ST_HiddenItem2)
        {
            ba.append((Int8 *) &item, 2);
            ba.push_back((Int8) hiddenID);
            ba.push_back((Int8) amount);
        }
        else
        {
            ba.append((Int8 *) &baseID, 4);
        }

        return ba;
    }
}
