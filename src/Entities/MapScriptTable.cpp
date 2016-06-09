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
#include <AME/Entities/Tables/EntityErrors.hpp>
#include <AME/Entities/Tables/MapScriptTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    MapScriptTable::MapScriptTable()
        : IUndoable(),
          m_Offset(0),
          m_Count(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    MapScriptTable::MapScriptTable(const MapScriptTable &rvalue)
        : IUndoable(),
          m_Offset(rvalue.m_Offset),
          m_Count(rvalue.m_Count)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    MapScriptTable &MapScriptTable::operator=(const MapScriptTable &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_Count = rvalue.m_Count;
        m_Scripts = rvalue.m_Scripts;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    MapScriptTable::~MapScriptTable()
    {
        foreach (MapScript *script, m_Scripts)
            delete script;

        for (int i = 0; i < s_UndoStack.size(); i++)
            delete s_UndoStack.at(i).script;

        for (int i = 0; i < s_RedoStack.size(); i++)
            delete s_RedoStack.at(i).script;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    // Comment:
    //
    // Fixed leak; allocating MapScript object but not freeing
    // it when throwing an error via AME_THROW.
    //
    ///////////////////////////////////////////////////////////
    bool MapScriptTable::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(MST_ERROR_OFFSET, rom.redirected());


        // Reads all level scripts until the termination sequence is reached
        while (true)
        {
            if (!rom.seek(offset + (m_Count++ * 5)))
                AME_THROW2(MST_ERROR_WHILE);


            // Checks for terminating sequence
            char type;
            if ((type = rom.readByte()) == 0)
            {
                break;
            }
            if (type > 7)
            {
                AME_SILENT_THROW(MST_ERROR_STYPE, rom.offset());
                continue;
            }

            // Reads the map script entry
            MapScript *script = new MapScript;
            script->type = static_cast<MapScriptType>(type);
            script->ptrVoid = rom.readPointer();


            // Determines whether the script-or-structure offset is valid
            if (!rom.seek(script->ptrVoid))
            {
                delete script;
                AME_THROW(MST_ERROR_SCRIPT, rom.offset() - 4);
            }

            // Determines the script type; if it is a structure, requires extra loading
            if (script->type == MST_HandlerEB0 || script->type == MST_HandlerF28)
            {
                // Reads EVERY auto-run script, unlike Advance-Map
                while (true)
                {
                    // Checks for terminating sequence
                    UInt16 variable;
                    if ((variable = rom.readHWord()) == 0)
                        break;

                    // Reads the entry
                    AutoScript autoScript;
                    autoScript.variable = variable;
                    autoScript.value = rom.readHWord();
                    autoScript.ptrScript = rom.readPointer();

                    // Determines whether the script offset is valid
                    if (!rom.checkOffset(autoScript.ptrScript))
                        AME_THROW(MST_ERROR_SCRIPT, rom.offset() - 4);

                    script->data.push_back(autoScript);
                    script->count++;
                }
            }


            // Increases table position
            m_Scripts.push_back(script);
        }


        // Loading successful
        m_Offset = offset;
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    bool MapScriptTable::requiresRepoint(const qboy::Rom &rom)
    {
        if (m_Scripts.size() < m_Count)
            return false;

        // Seeks to the end of the current table
        rom.seek(m_Offset + m_Count * 5);

        // Now determines whether there is enough trailing free space
        for (int i = 0; i < m_Scripts.size() - m_Count; i++)
        {
            if (!rom.canRead(5))
                return true;
            if (rom.readByte() != 0xFF || rom.readWord() != 0xFFFFFFFF)
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
    //
    ///////////////////////////////////////////////////////////
    QList<WriteEntry> MapScriptTable::write(UInt32 offset)
    {
        // Placeholder
        Q_UNUSED(offset);
        return QList<WriteEntry>();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<MapScript *> &MapScriptTable::scripts() const
    {
        return m_Scripts;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    void MapScriptTable::setScript(Int32 index, MapScript *script)
    {
        // Creates an undo entry for the old script
        UndoEntry undo;
        undo.index = index;
        undo.script = script;
        undo.action = URA_ACTION_MODIFY;
        s_UndoStack.push_back(undo);

        // Modifies the script
        m_Scripts[index] = script;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    void MapScriptTable::addScript(MapScript *script)
    {
        // Generates an undo entry from the given information
        UndoEntry undo;
        undo.script = script;
        undo.index = m_Scripts.size();
        undo.action = URA_ACTION_ADD;
        s_UndoStack.push_back(undo);

        // Now adds the actual script
        m_Scripts.push_back(script);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    void MapScriptTable::removeScript(Int32 index)
    {
        // Generates an undo entry from the given information
        UndoEntry undo;
        undo.script = m_Scripts[index];
        undo.index = index;
        undo.action = URA_ACTION_REMOVE;
        s_UndoStack.push_back(undo);

        // Now removes the script
        m_Scripts.removeAt(index);
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    void MapScriptTable::undo()
    {
        // Removes the action from the undo stack and
        // adds it to the redo stack afterwards.
        UndoEntry entry = s_UndoStack.last();
        s_UndoStack.removeLast();

        // Now reverses the process of the action
        if (entry.action == URA_ACTION_ADD)
        {
            // Removes the given script
            m_Scripts.removeAt(entry.index);
            s_RedoStack.push_back(entry);
        }
        else if (entry.action == URA_ACTION_REMOVE)
        {
            // Adds the given table again
            m_Scripts.insert(entry.index, entry.script);
            s_RedoStack.push_back(entry);
        }
        else
        {
            // Modifies the values within that entry
            // and reverses the process of modifying
            MapScript *old = entry.script;
            entry.script = m_Scripts[entry.index];
            m_Scripts[entry.index] = old;

            // Adds the modified entry to the redo stack
            s_RedoStack.push_back(entry);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    void MapScriptTable::redo()
    {
        // Removes the action from the redo stack and
        // adds it to the undo stack afterwards.
        UndoEntry entry = s_RedoStack.last();
        s_RedoStack.removeLast();

        // Now redoes the previous action
        if (entry.action == URA_ACTION_ADD)
        {
            // Adds the given script, as before
            m_Scripts.insert(entry.index, entry.script);
            s_UndoStack.push_back(entry);
        }
        else if (entry.action == URA_ACTION_REMOVE)
        {
            // Removes the given table, as before
            m_Scripts.removeAt(entry.index);
            s_UndoStack.push_back(entry);
        }
        else
        {
            // Modifies the values within that entry
            // and reverses the process of undoing
            MapScript *current = entry.script;
            entry.script = m_Scripts[entry.index];
            m_Scripts[entry.index] = current;

            // Adds the modified entry to the redo stack
            s_UndoStack.push_back(entry);
        }
    }
}
