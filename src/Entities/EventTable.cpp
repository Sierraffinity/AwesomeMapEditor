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
#include <AME/Entities/Tables/EventTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/6/2016
    //
    ///////////////////////////////////////////////////////////
    EventTable::EventTable()
        : IUndoable(),
          m_Offset(0),
          m_CountNpc(0),
          m_CountWarp(0),
          m_CountSign(0),
          m_CountTrigger(0),
          m_PtrNpc(0),
          m_PtrWarp(0),
          m_PtrSign(0),
          m_PtrTrigger(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/6/2016
    //
    ///////////////////////////////////////////////////////////
    EventTable::EventTable(const EventTable &rvalue)
        : IUndoable(),
          m_Offset(rvalue.m_Offset),
          m_CountNpc(rvalue.m_CountNpc),
          m_CountWarp(rvalue.m_CountWarp),
          m_CountSign(rvalue.m_CountSign),
          m_CountTrigger(rvalue.m_CountTrigger),
          m_PtrNpc(rvalue.m_PtrNpc),
          m_PtrWarp(rvalue.m_PtrWarp),
          m_PtrSign(rvalue.m_PtrSign),
          m_PtrTrigger(rvalue.m_PtrTrigger),
          m_Npcs(rvalue.m_Npcs),
          m_Warps(rvalue.m_Warps),
          m_Signs(rvalue.m_Signs),
          m_Triggers(rvalue.m_Triggers)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/6/2016
    //
    ///////////////////////////////////////////////////////////
    EventTable &EventTable::operator=(const EventTable &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_CountNpc = rvalue.m_CountNpc;
        m_CountWarp = rvalue.m_CountWarp;
        m_CountSign = rvalue.m_CountSign;
        m_CountTrigger = rvalue.m_CountTrigger;
        m_PtrNpc = rvalue.m_PtrNpc;
        m_PtrWarp = rvalue.m_PtrWarp;
        m_PtrSign = rvalue.m_PtrSign;
        m_PtrTrigger = rvalue.m_PtrTrigger;
        m_Npcs = rvalue.m_Npcs;
        m_Warps = rvalue.m_Warps;
        m_Signs = rvalue.m_Signs;
        m_Triggers = rvalue.m_Triggers;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/6/2016
    //
    ///////////////////////////////////////////////////////////
    EventTable::~EventTable()
    {
        foreach (Npc *npc, m_Npcs)
            delete npc;
        foreach (Warp *warp, m_Warps)
            delete warp;
        foreach (Sign *sign, m_Signs)
            delete sign;
        foreach (Trigger *trigger, m_Triggers)
            delete trigger;


        for (int i = 0; i < s_UndoStack.size(); i++)
        {
            void *event = s_UndoStack.at(i).event;
            switch (s_UndoStack.at(i).type)
            {
                case ET_Npc:     delete (Npc*)     event; break;
                case ET_Warp:    delete (Warp*)    event; break;
                case ET_Sign:    delete (Sign*)    event; break;
                case ET_Trigger: delete (Trigger*) event; break;
                case ET_Invalid:
                default: continue;
            }

        }

        for (int i = 0; i < s_RedoStack.size(); i++)
        {
            void *event = s_RedoStack.at(i).event;
            switch (s_RedoStack.at(i).type)
            {
                case ET_Npc:     delete (Npc*)     event; break;
                case ET_Warp:    delete (Warp*)    event; break;
                case ET_Sign:    delete (Sign*)    event; break;
                case ET_Trigger: delete (Trigger*) event; break;
                case ET_Invalid:
                default: continue;
            }

        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/6/2016
    //
    ///////////////////////////////////////////////////////////
    bool EventTable::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(EVT_ERROR_OFFSET, rom.redirected());


        // Reads the amount of events for each type
        m_CountNpc = rom.readByte();
        m_CountWarp = rom.readByte();
        m_CountTrigger = rom.readByte();
        m_CountSign = rom.readByte();

        // Reads all event type pointers
        m_PtrNpc = rom.readPointer();
        m_PtrWarp = rom.readPointer();
        m_PtrTrigger = rom.readPointer();
        m_PtrSign = rom.readPointer();


        // Determines whether all of these offsets are valid
        if (!rom.checkOffset(m_PtrNpc))
            AME_THROW(EVT_ERROR_SUBEVT, offset + 4);
        if (!rom.checkOffset(m_PtrWarp))
            AME_THROW(EVT_ERROR_SUBEVT, offset + 8);
        if (!rom.checkOffset(m_PtrTrigger))
            AME_THROW(EVT_ERROR_SUBEVT, offset + 12);
        if (!rom.checkOffset(m_PtrSign))
            AME_THROW(EVT_ERROR_SUBEVT, offset + 16);


        // Reads every non-playable character on the map
        rom.seek(m_PtrNpc);
        for (int i = 0; i < m_CountNpc; i++)
        {
            Npc *npc = new Npc;
            npc->npcID = rom.readByte();
            npc->imageID = rom.readByte();
            rom.readHWord(); // padding
            npc->positionX = rom.readHWord();
            npc->positionY = rom.readHWord();
            npc->level = rom.readByte();
            npc->behaviour = rom.readByte();
            npc->moveRadius = rom.readHWord();
            npc->property = rom.readByte();
            rom.readByte(); // padding
            npc->viewRadius = rom.readHWord();
            npc->ptrScript = rom.readPointer();
            npc->flag = rom.readHWord();
            rom.readHWord(); // padding
            m_Npcs.push_back(npc);

            // Determines whether the script offset is valid
            if (!rom.checkOffset(npc->ptrScript))
                AME_THROW(EVT_ERROR_SCRIPT, rom.offset() - 4);
        }


        // Reads all warp fields on the map
        rom.seek(m_PtrWarp);
        for (int i = 0; i < m_CountWarp; i++)
        {
            Warp *warp = new Warp;
            warp->positionX = rom.readHWord();
            warp->positionY = rom.readHWord();
            warp->level = rom.readByte();
            warp->warp = rom.readByte();
            warp->map = rom.readByte();
            warp->bank = rom.readByte();
            m_Warps.push_back(warp);
        }


        // Reads all trigger-fields on the map
        rom.seek(m_PtrTrigger);
        for (int i = 0; i < m_CountTrigger; i++)
        {
            Trigger *trigger = new Trigger;
            trigger->positionX = rom.readHWord();
            trigger->positionY = rom.readHWord();
            trigger->level = rom.readByte();
            rom.readByte(); // padding
            trigger->variable = rom.readHWord();
            trigger->value = rom.readHWord();
            rom.readHWord(); // padding
            trigger->ptrScript = rom.readPointer();
            m_Triggers.push_back(trigger);

            // Determines whether the script offset is valid
            if (!rom.checkOffset(trigger->ptrScript))
                AME_THROW(EVT_ERROR_SCRIPT, rom.offset() - 4);
        }


        // Reads all sign-posts on the map
        rom.seek(m_PtrSign);
        for (int i = 0; i < m_CountSign; i++)
        {
            UInt8 type;
            Sign *sign = new Sign;
            sign->positionX = rom.readHWord();
            sign->positionY = rom.readHWord();
            sign->level = rom.readByte();
            type = rom.readByte();
            rom.readHWord(); // padding

            // Can be either a script, item or secret base
            if (type >= ST_HiddenItem0 && type <= ST_HiddenItem2)
            {
                sign->item = rom.readHWord();
                sign->hiddenID = rom.readByte();
                sign->amount = rom.readByte();
            }
            else if (type <= ST_ScriptLeft)
            {
                sign->ptrScript = rom.readPointer();

                // Determines whether the script offset is valid
                if (!rom.checkOffset(sign->ptrScript))
                {
                    delete sign;
                    AME_THROW(EVT_ERROR_SCRIPT, rom.offset() - 4);
                }
            }
            else if (type == ST_SecretBase)
            {
                sign->baseID = rom.readWord();
            }
            else
            {
                delete sign;
                AME_THROW(EVT_ERROR_TYPE, rom.offset() - 8);
            }

            sign->type = static_cast<SignType>(type);
            m_Signs.push_back(sign);
        }


        // Loading successful
        m_Offset = offset;
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    EntityType EventTable::requiresRepoint(const qboy::Rom &rom)
    {
        int toRepoint = ET_Invalid;

        // The actual table does not need to be repointed
        if (m_Npcs.size() > m_CountNpc)
        {
            rom.seek(m_PtrNpc + m_CountNpc * 24);
            for (int i = 0; i < (m_Npcs.size() - m_CountNpc)*6; i++)
            {
                if (!rom.canRead(4) || rom.readWord() != 0xFFFFFFFF)
                {
                    toRepoint |= ET_Npc;
                    break;
                }
            }
        }
        if (m_Warps.size() > m_CountWarp)
        {
            rom.seek(m_PtrWarp + m_CountWarp * 8);
            for (int i = 0; i < (m_Warps.size() - m_CountWarp)*2; i++)
            {
                if (!rom.canRead(4) || rom.readWord() != 0xFFFFFFFF)
                {
                    toRepoint |= ET_Warp;
                    break;
                }
            }
        }
        if (m_Triggers.size() > m_CountTrigger)
        {
            rom.seek(m_PtrTrigger + m_CountTrigger * 16);
            for (int i = 0; i < (m_Triggers.size() - m_CountTrigger)*4; i++)
            {
                if (!rom.canRead(4) || rom.readWord() != 0xFFFFFFFF)
                {
                    toRepoint |= ET_Trigger;
                    break;
                }
            }
        }
        if (m_Signs.size() > m_CountSign)
        {
            rom.seek(m_PtrSign + m_CountSign * 12);
            for (int i = 0; i < (m_Signs.size() - m_CountSign)*3; i++)
            {
                if (!rom.canRead(4) || rom.readWord() != 0xFFFFFFFF)
                {
                    toRepoint |= ET_Sign;
                    break;
                }
            }
        }


        // Returns an OR combination of all types to be repointed
        return static_cast<EntityType>(toRepoint);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    QList<WriteEntry> EventTable::write(const QList<UInt32> &offsets)
    {
        /*
         * if (offsets[0] != 0) // repoint NPC table
         */

        // Placeholder
        Q_UNUSED(offsets);
        return QList<WriteEntry>();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Npc *> &EventTable::npcs() const
    {
        return m_Npcs;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Warp *> &EventTable::warps() const
    {
        return m_Warps;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Trigger *> &EventTable::triggers() const
    {
        return m_Triggers;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Sign *> &EventTable::signs() const
    {
        return m_Signs;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    void EventTable::setEvent(EntityType type, void *event)
    {
        // Creates an undo entry for the modification
        UndoEntry undo;
        undo.event = event;
        undo.type = type;
        undo.action = URA_ACTION_MODIFY;


        // Determines the list and index
        switch (type) {
        case ET_Npc:
            undo.index = m_Npcs.size();
            m_Npcs.push_back((Npc*)event);
            break;

        case ET_Warp:
            undo.index = m_Warps.size();
            m_Warps.push_back((Warp*)event);
            break;

        case ET_Trigger:
            undo.index = m_Triggers.size();
            m_Triggers.push_back((Trigger*)event);
            break;

        case ET_Sign:
            undo.index = m_Signs.size();
            m_Signs.push_back((Sign*)event);
            break;

        default: return;
        }


        // Adds the undo entry to the stack
        s_UndoStack.push_back(undo);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    void EventTable::addEvent(EntityType type, void *event)
    {
        Q_UNUSED(type);
        Q_UNUSED(event);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    void EventTable::removeEvent(Int32 index)
    {
        Q_UNUSED(index);
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    void EventTable::undo()
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    void EventTable::redo()
    {
    }
}
