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
#include <AME/Mapping/MappingErrors.hpp>
#include <AME/Mapping/MapBankTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    MapBankTable::MapBankTable()
        : /*IUndoable(),*/
          m_Offset(0),
          m_Count(-1)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    MapBankTable::MapBankTable(const MapBankTable &rvalue)
        : /*IUndoable(),*/
          m_Offset(rvalue.m_Offset),
          m_Count(rvalue.m_Count)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    MapBankTable &MapBankTable::operator=(const MapBankTable &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_Count = rvalue.m_Count;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    MapBankTable::~MapBankTable()
    {
        foreach (MapBank *bank, m_Banks)
            delete bank;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    bool MapBankTable::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(MBT_ERROR_OFFSET, rom.redirected());


        // Retrieves the amount of map banks
        while (true)
        {
            if (!rom.seek(offset + (++m_Count) * 4))
                AME_THROW(MBT_ERROR_WHILE, offset + m_Count * 4);

            // Determines whether the offset of the bank is valid
            unsigned bank = rom.readPointer();
            if (!rom.checkOffset(bank))
                break;

            // Determines whether the bank contains at least one map
            rom.seek(bank);
            if (!rom.checkOffset(rom.readPointer()))
                break;
        }


        // Reads all the banks
        for (unsigned i = 0; i < m_Count; i++)
        {
            rom.seek(offset + i * 4);

            // Retrieves the current and next bank
            UInt32 bankC = rom.readPointerRef();
            UInt32 bankN = rom.readPointer();
            MapBank *bank = new MapBank;

            if (!bank->read(rom, bankC, bankN))
            {
                delete bank;
                return false;
            }

            m_Banks.push_back(bank);
        }


        // Loading successful
        m_Offset = offset;
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 MapBankTable::offset() const
    {
        return m_Offset;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<MapBank *> &MapBankTable::banks() const
    {
        return m_Banks;
    }
}
