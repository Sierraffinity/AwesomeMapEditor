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


#ifndef __AME_CONNECTIONTABLE_HPP__
#define __AME_CONNECTIONTABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/System/IUndoable.hpp>
#include <AME/System/WriteEntry.hpp>
#include <AME/Entities/Connection.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    ConnectionTable.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/4/2016
    /// \brief   Holds all connections of a map.
    ///
    ///////////////////////////////////////////////////////////
    class ConnectionTable : public IUndoable {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::ConnectionTable.
        /// Note: ConnectionTable is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because of the undo/redo system.
        ///
        ///////////////////////////////////////////////////////////
        ConnectionTable();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::ConnectionTable.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        ConnectionTable(const ConnectionTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::ConnectionTable
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        ConnectionTable &operator=(const ConnectionTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated Connection objects.
        ///
        ///////////////////////////////////////////////////////////
        ~ConnectionTable();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read all connections on the map.
        ///
        /// Reads all connections until the specified amount of
        /// connections were read or if an invalid connection
        /// was encountered.
        ///
        /// \param rom Currently opened ROM file
        /// \param offset Offset of the connection table
        /// \returns true if all scripts were read correctly.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset);

        ///////////////////////////////////////////////////////////
        /// \brief Determines whether a repoint is required.
        ///
        /// Compares the current connection count to the
        /// old count. If it is bigger, the table requires a repoint
        /// except if there are enough trailing 0xFF bytes.
        ///
        ///////////////////////////////////////////////////////////
        bool requiresRepoint(const qboy::Rom &rom);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the connections to the given offset.
        ///
        /// First make sure the object requires a repoint by
        /// calling ConnectionTable::requiresRepoint and then
        /// call this function to write the data to the given
        /// new offset. Call the overloaded non-argument function
        /// to write the data to the same offset as before.
        ///
        /// \param offset New offset of the data (optional)
        /// \returns a list of write entries to use for the rom.
        ///
        ///////////////////////////////////////////////////////////
        QList<WriteEntry> write(UInt32 offset = 0);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a reference to all the connections.
        ///
        /// The connections may only be modified through the
        /// ConnectionTable::setConnection function.
        ///
        /// \returns a constant reference to the connections.
        ///
        ///////////////////////////////////////////////////////////
        const QList<Connection *> &connections() const;


        ///////////////////////////////////////////////////////////
        /// \brief Modifies one connection within the table.
        /// \param index Index of the connection to modify
        /// \param entry Connection structure to replace
        ///
        ///////////////////////////////////////////////////////////
        void setConnection(Int32 index, Connection *entry);

        ///////////////////////////////////////////////////////////
        /// \brief Add a connection to the table.
        ///
        /// This operation will most likely cause the table to
        /// repoint itself, unless having enough trailing free space.
        ///
        /// \param script Connection structure to be added
        ///
        ///////////////////////////////////////////////////////////
        void addConnection(Connection *entry);

        ///////////////////////////////////////////////////////////
        /// \brief Removes a connection from the table.
        ///
        /// This operation does not repoint the table but cleares
        /// the unused space after the table with a bunch of 0xFF's.
        ///
        /// \param index Index of the connection to remove
        ///
        ///////////////////////////////////////////////////////////
        void removeConnection(Int32 index);


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
            Connection *entry;      ///< Holds the actual entry
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
        Int32 m_Count;                  ///< Connection amount
        UInt32 m_PtrData;               ///< Offset of the data
        QList<Connection *> m_Connections;
    };
}


#endif // __AME_CONNECTIONTABLE_HPP__
