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
#include <AME/Entities/Tables/EntityErrors.hpp>
#include <AME/Entities/Tables/ConnectionTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    ConnectionTable::ConnectionTable()
        : IUndoable(),
          m_Offset(0),
          m_Count(0),
          m_PtrData(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    ConnectionTable::ConnectionTable(const ConnectionTable &rvalue)
        : IUndoable(),
          m_Offset(rvalue.m_Offset),
          m_Count(rvalue.m_Count),
          m_PtrData(rvalue.m_PtrData)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    ConnectionTable &ConnectionTable::operator=(const ConnectionTable &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_Count = rvalue.m_Count;
        m_PtrData = rvalue.m_PtrData;
        m_Connections = rvalue.m_Connections;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    ConnectionTable::~ConnectionTable()
    {
        foreach (Connection *entry, m_Connections)
            delete entry;

        for (int i = 0; i < s_UndoStack.size(); i++)
            delete s_UndoStack.at(i).entry;

        for (int i = 0; i < s_RedoStack.size(); i++)
            delete s_RedoStack.at(i).entry;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    bool ConnectionTable::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(COT_ERROR_OFFSET, rom.redirected());


        // Reads the amount of map connections and their location
        m_Count = rom.readWord();
        m_PtrData = rom.readPointer();

        // Determines whether the location is valid
        if (!rom.seek(m_PtrData))
            AME_THROW(COT_ERROR_OFFDAT, offset + 4);


        // Attempts to read all connections
        for (int i = 0; i < m_Count; i++)
        {
            Connection *entry = new Connection;
            UInt8 direction = rom.readByte();
            rom.readBytes(3); // padding
            entry->offset = rom.readWord();
            entry->bank = rom.readByte();
            entry->map = rom.readByte();
            rom.readHWord(); // padding

            // Determines whether the direction is valid
            if (direction > DIR_Emerge)
            {
                AME_SILENT_THROW(COT_ERROR_DIRVAL, m_PtrData);
                delete entry;
                continue;
            }
            else
            {
                entry->direction = static_cast<DirectionType>(direction);
                m_Connections.push_back(entry);
            }
        }


        // Loading successful
        m_Offset = offset;
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    bool ConnectionTable::requiresRepoint(const qboy::Rom &rom)
    {
        if (m_Connections.size() < m_Count)
            return false;

        // Seeks to the end of the _logical_ table.
        // Note: The head-table <count><ptr> will always have the
        // same size, just <ptr> might be changed upon repointing
        rom.seek(m_PtrData + m_Count * 12);

        // Determines whether there is enough trailing free space
        for (int i = 0; i < m_Connections.size() - m_Count; i++)
        {
            if (!rom.canRead(12))
                return true;
            if (rom.readWord() != 0xFFFFFFFF ||
                rom.readWord() != 0xFFFFFFFF ||
                rom.readWord() != 0xFFFFFFFF)
                return true;
        }

        // Iteration survived; enough space
        return false;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    QList<WriteEntry> ConnectionTable::write(UInt32 offset)
    {
        // Placeholder
        Q_UNUSED(offset);
        return QList<WriteEntry>();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Connection *> &ConnectionTable::connections() const
    {
        return m_Connections;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    void ConnectionTable::setConnection(Int32 index, Connection *entry)
    {
        // Creates an undo entry for the old connection
        UndoEntry undo;
        undo.index = index;
        undo.entry = entry;
        undo.action = URA_ACTION_MODIFY;
        s_UndoStack.push_back(undo);

        // Modifies the connection
        m_Connections[index] = entry;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    void ConnectionTable::addConnection(Connection *entry)
    {
        // Generates an undo entry from the given information
        UndoEntry undo;
        undo.entry = entry;
        undo.index = m_Connections.size();
        undo.action = URA_ACTION_ADD;
        s_UndoStack.push_back(undo);

        // Now adds the actual connection
        m_Connections.push_back(entry);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    void ConnectionTable::removeConnection(Int32 index)
    {
        // Generates an undo entry from the given information
        UndoEntry undo;
        undo.entry = m_Connections[index];
        undo.index = index;
        undo.action = URA_ACTION_REMOVE;
        s_UndoStack.push_back(undo);

        // Now removes the connection
        m_Connections.removeAt(index);
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    void ConnectionTable::undo()
    {
        // Removes the action from the undo stack and
        // adds it to the redo stack afterwards.
        UndoEntry entry = s_UndoStack.last();
        s_UndoStack.removeLast();

        // Now reverses the process of the action
        if (entry.action == URA_ACTION_ADD)
        {
            // Removes the given connection
            m_Connections.removeAt(entry.index);
            s_RedoStack.push_back(entry);
        }
        else if (entry.action == URA_ACTION_REMOVE)
        {
            // Adds the given table again
            m_Connections.insert(entry.index, entry.entry);
            s_RedoStack.push_back(entry);
        }
        else
        {
            // Modifies the values within that entry
            // and reverses the process of modifying
            Connection *old = entry.entry;
            entry.entry = m_Connections[entry.index];
            m_Connections[entry.index] = old;

            // Adds the modified entry to the redo stack
            s_RedoStack.push_back(entry);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    void ConnectionTable::redo()
    {
        // Removes the action from the redo stack and
        // adds it to the undo stack afterwards.
        UndoEntry entry = s_RedoStack.last();
        s_RedoStack.removeLast();

        // Now redoes the previous action
        if (entry.action == URA_ACTION_ADD)
        {
            // Adds the connection, as before
            m_Connections.insert(entry.index, entry.entry);
            s_UndoStack.push_back(entry);
        }
        else if (entry.action == URA_ACTION_REMOVE)
        {
            // Removes the connection, as before
            m_Connections.removeAt(entry.index);
            s_UndoStack.push_back(entry);
        }
        else
        {
            // Modifies the values within that entry
            // and reverses the process of undoing
            Connection *current = entry.entry;
            entry.entry = m_Connections[entry.index];
            m_Connections[entry.index] = current;

            // Adds the modified entry to the redo stack
            s_UndoStack.push_back(entry);
        }
    }
}
