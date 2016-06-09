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
#include <AME/Structures/StructureErrors.hpp>
#include <AME/Structures/WildPokemonTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
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
    // Contributors:   Pokedude
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
    // Contributors:   Pokedude
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
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonTable::~WildPokemonTable()
    {
        foreach (WildPokemonSubTable *subTable, m_Tables)
            delete subTable;

        for (int i = 0; i < s_UndoStack.size(); i++)
            delete s_UndoStack.at(i).table;

        for (int i = 0; i < s_RedoStack.size(); i++)
            delete s_RedoStack.at(i).table;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
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
        m_Offset = offset;
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
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
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    // Comment:
    //
    // Added the ending sequence (0x0000FFFF) to the entries.
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
            for (int i = 0; i < m_Count * 4; i++)
                clearEntry.data.push_back(0xFF);

            entries.push_back(clearEntry);

            // Adds an undo entry for the repoint
            UndoEntry repEntry;
            repEntry.action = URA_ACTION_REPOINT;
            repEntry.offset = m_Offset;
            s_UndoStack.append(repEntry);

            // Sets the repointed offset as new offset
            m_Offset = offset;
        }

        // If table is smaller than before, clear unused space
        if (m_Tables.size() < m_Count)
        {
            WriteEntry clearEntry { m_Offset + m_Tables.size() * 4 };
            for (int i = 0; i < m_Count - m_Tables.size(); i++)
                clearEntry.data.push_back((char)0xFF);

            entries.append(clearEntry);
        }


        // Writes all the pointers to the table
        WriteEntry tableEntry { m_Offset };
        for (int i = 0; i < m_Tables.size(); i++)
        {
            WildPokemonSubTable *sub = m_Tables[i];
            tableEntry.addPointer(sub->offset());
            entries.append(sub->write());
        }


        // Appends all table pointers as one entry
        tableEntry.addWord(0x0000FFFF);
        entries.append(tableEntry);
        return entries;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
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
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    // Comment:
    //
    // Fixed the index; previously took s_UndoStack.size.
    //
    ///////////////////////////////////////////////////////////
    void WildPokemonTable::add(WildPokemonSubTable *subTable)
    {
        // Generates an undo entry out of the given information
        UndoEntry entry;
        entry.table = subTable;
        entry.index = m_Tables.size();
        entry.action = URA_ACTION_ADD;
        s_UndoStack.append(entry);

        // Now adds the sub-table to the actual head table
        m_Tables.append(subTable);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
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
    // Contributors:   Pokedude
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

        // Now reverses the process of the action
        if (entry.action == URA_ACTION_ADD)
        {
            // Removes the given table
            m_Tables.removeAt(entry.index);
            s_RedoStack.append(entry);
        }
        else if (entry.action == URA_ACTION_REMOVE)
        {
            // Adds the given table again
            m_Tables.insert(entry.index, entry.table);
            s_RedoStack.append(entry);
        }
        else
        {
            // Replaces the new offset with the old one
            UInt32 newOffset = m_Offset;
            m_Offset = entry.offset;
            entry.offset = newOffset;
            s_RedoStack.append(entry);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
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

        // Now does the same action again
        if (entry.action == URA_ACTION_ADD)
        {
            // Adds the given table, as before
            m_Tables.insert(entry.index, entry.table);
            s_UndoStack.append(entry);
        }
        else if (entry.action == URA_ACTION_REMOVE)
        {
            // Removes the given table, as before
            m_Tables.removeAt(entry.index);
            s_UndoStack.append(entry);
        }
        else
        {
            // Replaces the old offset with the new one
            UInt32 oldOffset = m_Offset;
            m_Offset = entry.offset;
            entry.offset = oldOffset;
            s_UndoStack.append(entry);
        }
    }
}
