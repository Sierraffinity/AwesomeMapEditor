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
#include <AME/Structures/WildPokemonArea.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonArea::WildPokemonArea()
        : IUndoable(),
          m_Probability(0),
          m_Offset(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonArea::WildPokemonArea(const WildPokemonArea &rvalue)
        : IUndoable(),
          m_Probability(rvalue.m_Probability),
          m_Offset(rvalue.m_Offset),
          m_Entries(rvalue.m_Entries)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonArea &WildPokemonArea::operator=(const WildPokemonArea &rvalue)
    {
        m_Probability = rvalue.m_Probability;
        m_Offset = rvalue.m_Offset;
        m_Entries = rvalue.m_Entries;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonArea::~WildPokemonArea()
    {
        foreach (WildPokemonEncounter *entry, m_Entries)
            delete entry;

        for (int i = 0; i < s_UndoStack.size(); i++)
            delete s_UndoStack.at(i).entry;

        for (int i = 0; i < s_RedoStack.size(); i++)
            delete s_RedoStack.at(i).entry;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    bool WildPokemonArea::read(const qboy::Rom &rom, UInt32 offset, UInt32 amount)
    {
        rom.seek(offset); // exception-safe this time

        // Reads the probability and the pointer to the encounter array
        m_Probability = rom.readByte();
        rom.readBytes(3); // padding
        m_Offset =  rom.readPointer();


        // Determines whether the encounter array offset is valid
        if (!rom.checkOffset(m_Offset))
            AME_THROW(WPST_ERROR_DATA, offset + 4);


        // Reads the encounter array
        for (UInt32 i = 0; i < amount; i++)
        {
            WildPokemonEncounter *entry = new WildPokemonEncounter;
            entry->min = rom.readByte();
            entry->max = rom.readByte();
            entry->id = rom.readHWord();
            m_Entries.push_back(entry);
        }


        // Loading successful
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    QList<WriteEntry> WildPokemonArea::write(UInt32 offset)
    {
        QList<WriteEntry> entries;
        WriteEntry tableEntry;


        // Adds the table structure to the entries
        tableEntry.offset = offset;
        tableEntry.data.push_back(m_Probability);
        tableEntry.data.push_back((char)0); // padding
        tableEntry.data.push_back((char)0); // padding
        tableEntry.data.push_back((char)0); // padding
        tableEntry.addPointer(m_Offset);
        entries.push_back(tableEntry);


        // Adds the encounter array to the entries
        WriteEntry pokeEntry;
        pokeEntry.offset = m_Offset;

        for (int i = 0; i < m_Entries.size(); i++)
        {
            const WildPokemonEncounter *entry = m_Entries.at(i);
            pokeEntry.data.push_back(entry->min);
            pokeEntry.data.push_back(entry->max);
            pokeEntry.addHWord(entry->id);
        }


        // Adds all continuous encounters as one combined entry
        entries.push_back(pokeEntry);
        return entries;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 WildPokemonArea::offset() const
    {
        return m_Offset;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    UInt8 WildPokemonArea::probability() const
    {
        return m_Probability;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<WildPokemonEncounter *> &WildPokemonArea::entries() const
    {
        return m_Entries;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    void WildPokemonArea::setEntry(Int32 index, WildPokemonEncounter *entry)
    {
        // Creates an undo entry for the old encounter
        UndoEntry undo;
        undo.index = index;
        undo.entry = m_Entries[index];
        s_UndoStack.push_back(undo);

        // Modifies the encounter
        m_Entries[index] = entry;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    void WildPokemonArea::undo()
    {
        // Removes the last undo entry
        UndoEntry entry = s_UndoStack.last();
        s_UndoStack.removeLast();


        // Modifies the values within that entry and
        // reverses the process of modifying
        WildPokemonEncounter *old = entry.entry;
        entry.entry = m_Entries[entry.index];
        m_Entries[entry.index] = old;


        // Adds the modified entry to the redo stack
        s_RedoStack.push_back(entry);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    void WildPokemonArea::redo()
    {
        // Removes the last redo entry
        UndoEntry entry = s_RedoStack.last();
        s_RedoStack.removeLast();


        // Modifies the values within that entry and
        // reverse the process of undoing
        WildPokemonEncounter *current = entry.entry;
        entry.entry = m_Entries[entry.index];
        m_Entries[entry.index] = current;


        // Adds the modified entry to the undo stack
        s_UndoStack.push_back(entry);
    }
}
