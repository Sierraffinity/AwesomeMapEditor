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
#include <AME/Text/String.hpp>
#include <AME/Structures/StructureErrors.hpp>
#include <AME/Structures/ItemTable.hpp>
#include <AME/System/Configuration.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    ItemTable::ItemTable()
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    ItemTable::ItemTable(const ItemTable &rvalue)
        : m_Names(rvalue.m_Names)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    ItemTable &ItemTable::operator=(const ItemTable &rvalue)
    {
        m_Names = rvalue.m_Names;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    ItemTable::~ItemTable()
    {
        m_Names.clear();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    bool ItemTable::read(const qboy::Rom &rom)
    {
        // Checks the configuration pointer
        if (!rom.checkOffset(CONFIG(ItemData)))
            AME_THROW2(ITM_ERROR_DATA);

        // Attempts to read all the item names
        for (unsigned i = 0; i < CONFIG(ItemCount); i++)
        {
            m_Names.push_back(String::read(rom, CONFIG(ItemData) + i * 0x2C));
        }

        // Loading successful
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    const QStringList &ItemTable::names() const
    {
        return m_Names;
    }
}
