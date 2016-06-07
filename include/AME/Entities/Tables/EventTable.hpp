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


#ifndef __AME_EVENTTABLE_HPP__
#define __AME_EVENTTABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/System/IUndoable.hpp>
#include <AME/System/WriteEntry.hpp>
#include <AME/Entities/Npc.hpp>
#include <AME/Entities/Warp.hpp>
#include <AME/Entities/Sign.hpp>
#include <AME/Entities/Trigger.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    EventTable.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/6/2016
    /// \brief   Holds all events that are located on the map.
    ///
    ///////////////////////////////////////////////////////////
    class EventTable : public IUndoable {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::EventTable.
        /// Note: EventTable is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because of the undo/redo system.
        ///
        ///////////////////////////////////////////////////////////
        EventTable();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::EventTable.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        EventTable(const EventTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::EventTable
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        EventTable &operator=(const EventTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated event objects.
        ///
        ///////////////////////////////////////////////////////////
        ~EventTable();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read all the events on the map.
        ///
        /// Reads all warps, sign-posts, NPCs and trigger fields.
        ///
        /// \param rom Currently opened ROM file
        /// \param offset Offset of the event table
        /// \returns true if all events were read correctly.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset);


        ///////////////////////////////////////////////////////////
        /// \brief Determines whether a repoint is required.
        ///
        /// Compares the current event count to the
        /// old count. If it is bigger, the table requires a repoint
        /// except if there are enough trailing 0xFF bytes.
        ///
        /// \returns a combo of types that need to be repointed.
        ///
        ///////////////////////////////////////////////////////////
        EntityType requiresRepoint(const qboy::Rom &rom);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the event table to the given offset.
        ///
        /// Each sub-table may require a repoint. To check, call
        /// the function EventTable::requiresRepoint and determine
        /// which table-types need to be repointed by ANDing the
        /// return value with the ET enum values. The parameter
        /// must be a list with the following entries:
        /// { npc-offset, warp-offset, trigger-offset, sign-offset }
        /// If one of these entries does not require a repoint,
        /// simply place a nullptr instead. Example:
        /// { 0xABCDEF, 0x0, 0xFEDCAB, 0x0 }
        ///
        /// \param offsets List of repointed type offsets
        /// \returns a list of write entries to use for the rom.
        ///
        ///////////////////////////////////////////////////////////
        QList<WriteEntry> write(const QList<UInt32> &offsets);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a reference to all NPCs.
        ///
        ///////////////////////////////////////////////////////////
        const QList<Npc *> &npcs() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a reference to all warps.
        ///
        ///////////////////////////////////////////////////////////
        const QList<Warp *> &warps() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a reference to all triggers.
        ///
        ///////////////////////////////////////////////////////////
        const QList<Trigger *> &triggers() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a reference to all sign-posts.
        ///
        ///////////////////////////////////////////////////////////
        const QList<Sign *> &signs() const;


        ///////////////////////////////////////////////////////////
        /// \brief Modifies one event within the table.
        /// \param type Type of the event
        /// \param event Anonymous pointer to the event struct
        ///
        ///////////////////////////////////////////////////////////
        void setEvent(EntityType type, void *event);

        ///////////////////////////////////////////////////////////
        /// \brief Adds an event to the table.
        ///
        /// This operation will most likely cause the table to
        /// repoint itself, unless having enough trailing free space.
        ///
        /// \param type Type of the event
        /// \param event Anonymous pointer to the event struct
        ///
        ///////////////////////////////////////////////////////////
        void addEvent(EntityType type, void *event);

        ///////////////////////////////////////////////////////////
        /// \brief Removes an event from the table.
        ///
        /// This operation does not repoint the table but cleares
        /// the unused space after the table with a bunch of 0xFF's.
        ///
        /// \param index Index of the script to remove
        ///
        ///////////////////////////////////////////////////////////
        void removeEvent(Int32 index);


        ///////////////////////////////////////////////////////////
        /// \brief Undoes the most recent modifying action
        ///
        ///////////////////////////////////////////////////////////
        void undo() Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Redoes the most recently undone action.
        ///
        ///////////////////////////////////////////////////////////
        void redo() Q_DECL_OVERRIDE;


    protected:

        ///////////////////////////////////////////////////////////
        struct UndoEntry
        {
            void *event;            ///< Holds the actual entry
            EntityType type;        ///< Holds the event type
            UndoRedoAction action;  ///< Holds the causing action
            Int32 index;            ///< Holds the entry position
            UInt32 offset;          ///< Holds the old offset
        };


        ///////////////////////////////////////////////////////////
        // Undo/redo variables
        //
        ///////////////////////////////////////////////////////////
        QList<UndoEntry> s_UndoStack;
        QList<UndoEntry> s_RedoStack;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt32 m_Offset;                ///< Offset of the table
        UInt8 m_CountNpc;               ///< Amount of NPCs
        UInt8 m_CountWarp;              ///< Amount of warps
        UInt8 m_CountSign;              ///< Amount of signs
        UInt8 m_CountTrigger;           ///< Amount of triggers
        UInt32 m_PtrNpc;                ///< Offset of NPC data
        UInt32 m_PtrWarp;               ///< Offset of warp data
        UInt32 m_PtrSign;               ///< Offset of sign data
        UInt32 m_PtrTrigger;            ///< Offset of trigger data
        QList<Npc *> m_Npcs;            ///< Holds all NPCs
        QList<Warp *> m_Warps;          ///< Holds all warps
        QList<Sign *> m_Signs;          ///< Holds all signs
        QList<Trigger *> m_Triggers;    ///< Holds all triggers
    };
}


#endif // __AME_EVENTTABLE_HPP__
