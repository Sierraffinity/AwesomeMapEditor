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


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/System/ErrorStack.hpp>
#include <AME/Structures/StructureErrors.hpp>
#include <AME/Structures/WildPokemonTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   6/3/2016
    /// \brief  Stores an undo- or redo entry.
    ///
    ///////////////////////////////////////////////////////////
    struct UndoEntry
    {
        WildPokemonSubTable *table; ///< Holds the actual entry
        UndoRedoAction action;      ///< Holds the causing action
        Int32 index;                ///< Holds the entry position
    };


    ///////////////////////////////////////////////////////////
    // Defines variables exclusively used by this code file
    //
    ///////////////////////////////////////////////////////////
    QList<UndoEntry> s_UndoStack;
    QList<UndoEntry> s_RedoStack;


    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonTable::WildPokemonTable()
        : IUndoable(),
          m_Offset(0),
          m_Count(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonTable::WildPokemonTable(const WildPokemonTable &rvalue)
        : IUndoable(),
          m_Offset(rvalue.m_Offset),
          m_Count(rvalue.m_Count),
          m_Tables(rvalue.m_Tables)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonTable &WildPokemonTable::operator=(const WildPokemonTable &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_Count = rvalue.m_Count;
        m_Tables = rvalue.m_Tables;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonTable::~WildPokemonTable()
    {
        foreach (WildPokemonSubTable *subTable, m_Tables)
            delete subTable;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    bool WildPokemonTable::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(WPT_ERROR_OFFSET, rom.redirected());


        // Retrieves the amount of sub-tables in the head table
        while (true)
        {
            if (!rom.seek(offset + m_Count * 20))
                AME_THROW2(WPT_ERROR_RANGE);

            // Determines whether the ending sequence (0xFFFF0000) is reached.
            if (rom.readWord() == 0x0000FFFF)
                break;
            else
                m_Count++;
        }


        // Reads all the wildpokemon sub-tables
        for (int i = 0; i < m_Count; i++)
        {
            WildPokemonSubTable *sub = new WildPokemonSubTable;
            if (!sub->read(rom, offset + i * 20))
                return false;

            m_Tables.push_back(sub);
        }


        // Loading successful
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    bool WildPokemonTable::requiresRepoint(const qboy::Rom &rom)
    {
        if (m_Tables.size() < m_Count)
            return false;

        // Seeks to the end of the current table
        rom.seek(m_Offset + m_Count * 20);

        // Now determines whether there is enough trailing free space
        for (int i = 0; i < m_Tables.size() - m_Count; i++)
        {
            if (!rom.canRead(VT_Word))
                return true;
            if (rom.readWord() != 0xFFFFFFFF)
                return true;
        }

        // Iteration survived; enough space
        return false;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    QList<WriteEntry> WildPokemonTable::write(UInt32 offset)
    {
        QList<WriteEntry> entries;


        // If offset is a repointed offset, clear old data with 0xFF
        if (offset > 0)
        {
            // Each pointer is sized four bytes
            WriteEntry clearEntry { m_Offset };
            for (int i = 0; i < m_Count*4; i++)
                clearEntry.data.push_back(0xFF);

            entries.push_back(clearEntry);
            m_Offset = offset;
            /* TODO: Undo/Redo for repoint */
        }


        // Writes all the pointers to the table
        WriteEntry tableEntry { m_Offset };
        for (int i = 0; i < m_Tables.size(); i++)
        {
            const WildPokemonSubTable *sub = m_Tables.at(i);
            tableEntry.addPointer(sub->offset());
            entries.append(sub->write());
        }


        // Appends all table pointers as one entry
        entries.append(tableEntry);
        return entries;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    QList<WildPokemonSubTable *> &WildPokemonTable::tables()
    {
        return m_Tables;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    void WildPokemonTable::add(WildPokemonSubTable *subTable)
    {
        // Generates an undo entry out of the given information
        UndoEntry entry;
        entry.table = subTable;
        entry.index = s_UndoStack.size();
        entry.action = URA_ACTION_ADD;
        s_UndoStack.append(entry);

        // Now adds the sub-table to the actual head table
        m_Tables.append(subTable);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    void WildPokemonTable::remove(Int32 index)
    {
        // Generates an undo entry out of the given information
        UndoEntry entry;
        entry.table = m_Tables[index];
        entry.index = index;
        entry.action = URA_ACTION_REMOVE;
        s_UndoStack.append(entry);

        // Now removes the table at the given index
        m_Tables.removeAt(index);
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    void WildPokemonTable::undo()
    {
        // Removes the action from the undo stack and
        // adds it to the redo stack afterwards.
        UndoEntry entry = s_UndoStack.last();
        s_UndoStack.removeLast();
        s_RedoStack.append(entry);

        // Now reverses the process of the action
        if (entry.action == URA_ACTION_ADD)
        {
            // Removes the given table
            m_Tables.removeAt(entry.index);
        }
        else
        {
            // Adds the given table again
            m_Tables.insert(entry.index, entry.table);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    void WildPokemonTable::redo()
    {
        // Removes the action from the redo stack and
        // adds it to the undo stack afterwards.
        UndoEntry entry = s_RedoStack.last();
        s_RedoStack.removeLast();
        s_UndoStack.append(entry);

        // Now does the same action again
        if (entry.action == URA_ACTION_ADD)
        {
            // Adds the given table, as before
            m_Tables.insert(entry.index, entry.table);
        }
        else
        {
            // Removes the given table, as before
            m_Tables.removeAt(entry.index);
        }
    }
}
