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


#ifndef __AME_WILDPOKEMONTABLE_HPP__
#define __AME_WILDPOKEMONTABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/System/IUndoable.hpp>
#include <AME/System/WriteEntry.hpp>
#include <AME/Structures/WildPokemonSubTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    WildPokemonTable.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/2/2016
    /// \brief   Holds all wild pokémon tables within the game.
    ///
    /// The wild pokémon table consists of sub-tables which
    /// again hold four fixed-size tables for each area in
    /// which the Pokémon can appear.
    ///
    ///////////////////////////////////////////////////////////
    class WildPokemonTable : public IUndoable {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::WildPokemonTable.
        /// Note: WildPokemonTable is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because wild pokemon tables are
        /// unique.
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonTable();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::WildPokemonTable.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonTable(const WildPokemonTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::WildPokemonTable
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonTable &operator=(const WildPokemonTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated WildPokemonSubTable objects.
        ///
        ///////////////////////////////////////////////////////////
        ~WildPokemonTable();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read all the sub-tables.
        ///
        /// Reads each sub-table step-by-step. Returns false if one
        /// sub-table could not be read correctly. The error which
        /// caused the loading to crash will be stored on the error
        /// stack and output at the end of the parental function.
        ///
        /// \param rom Currently opened ROM file
        /// \param offset The offset of the head table
        /// \returns true on success.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset);


        ///////////////////////////////////////////////////////////
        /// \brief Determines whether a repoint is required.
        ///
        /// Compares the current wild pokemon table count to the
        /// old count. If it is bigger, the table requires a repoint
        /// except if there are enough trailing 0xFF bytes.
        ///
        ///////////////////////////////////////////////////////////
        bool requiresRepoint(const qboy::Rom &rom);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the sub-tables to the given offset.
        ///
        /// First make sure the object requires a repoint by
        /// calling WildPokemontable::requiresRepoint and then
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
        /// \brief Retrieves reference to all sub-tables.
        ///
        /// This object is only for read access. Add or remove
        /// entries through the respective functions. The Pokémon
        /// encounters within the table may be modified directly.
        ///
        /// \returns a reference to the sub-tables.
        ///
        ///////////////////////////////////////////////////////////
        QList<WildPokemonSubTable *> &tables();

        ///////////////////////////////////////////////////////////
        /// \brief Adds a sub-table to the head table.
        ///
        /// This operation may cause the table to repoint itself
        /// due to lack of space after the table.
        ///
        /// \param subTable Pointer to new WildPokemonSubTable
        ///
        ///////////////////////////////////////////////////////////
        void add(WildPokemonSubTable *subTable);

        ///////////////////////////////////////////////////////////
        /// \brief Removes a sub-table from the head table.
        ///
        /// This operation does not repoint the table, but cleares
        /// four bytes of space as one pointer is deleted.
        ///
        /// \param index Index of the sub-table to remove
        ///
        ///////////////////////////////////////////////////////////
        void remove(Int32 index);


        ///////////////////////////////////////////////////////////
        /// \brief Undoes the most recent add or remove action.
        ///
        /// Either undoes the adding of a table entry or the
        /// removal of a table entry.
        ///
        ///////////////////////////////////////////////////////////
        void undo() Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Redoes the most recently undone action.
        ///
        /// Either redoes the adding of a table entry or the
        /// removal of a table entry.
        ///
        ///////////////////////////////////////////////////////////
        void redo() Q_DECL_OVERRIDE;


    protected:

        ///////////////////////////////////////////////////////////
        struct UndoEntry
        {
            WildPokemonSubTable *table; ///< Holds the actual entry
            UndoRedoAction action;      ///< Holds the causing action
            Int32 index;                ///< Holds the entry position
            UInt32 offset;              ///< Holds the old offset
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
        UInt32 m_Offset;           ///< Offset of the head table
        Int32 m_Count;             ///< Initial count of sub-tables
        QList<WildPokemonSubTable *> m_Tables;
    };
}


#endif // __AME_WILDPOKEMONTABLE_HPP__
