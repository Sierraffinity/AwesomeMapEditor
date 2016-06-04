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


#ifndef __AME_WILDPOKEMONAREA_HPP__
#define __AME_WILDPOKEMONAREA_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/System/IUndoable.hpp>
#include <AME/System/WriteEntry.hpp>
#include <AME/Structures/WildPokemonEncounter.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    WildPokemonArea.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/4/2016
    /// \brief   Contains one area with different encounters.
    ///
    /// The area contains the probability that you will
    /// encounter a Pokémon on every step. It also consists
    /// of a fixed amount of Pokémon that can be encountered.
    ///
    ///////////////////////////////////////////////////////////
    class WildPokemonArea : public IUndoable {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Sets the probability and the offset to zero.
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonArea();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::WildPokemonArea.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonArea(const WildPokemonArea &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::WildPokemonArea
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonArea &operator=(const WildPokemonArea &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated WildPokemonEncounter objects.
        ///
        ///////////////////////////////////////////////////////////
        ~WildPokemonArea();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read all the encounters
        ///
        /// Each array has a fixed size and consists of several
        /// entries that define min/max level and Pokémon ID.
        ///
        /// \param rom Currently opened ROM file
        /// \param offset The offset of the encounter array
        /// \param amount Size of the encounter array
        /// \returns true on success.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset, UInt32 amount);

        ///////////////////////////////////////////////////////////
        /// \brief Writ<es the encounters to the same offset.
        ///
        /// As area arrays have a fixed size, they do not require
        /// a repoint.
        ///
        /// \param offset Location of this structure
        /// \returns a list of write entries to use for the rom.
        ///
        ///////////////////////////////////////////////////////////
        QList<WriteEntry> write(UInt32 offset);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the encounter array offset.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 offset() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the probability for an encounter.
        ///
        ///////////////////////////////////////////////////////////
        UInt8 probability() const;


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a constant reference to the entries.
        /// \note Use WildPokemonArea::setEntry to change entries.
        ///
        ///////////////////////////////////////////////////////////
        const QList<WildPokemonEncounter *> &entries() const;

        ///////////////////////////////////////////////////////////
        /// \brief Modifies one encounter array entry.
        /// \param index Position of the counter to change
        /// \param entry Entry containing the values to change
        ///
        ///////////////////////////////////////////////////////////
        void setEntry(Int32 index, WildPokemonEncounter *entry);


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
            WildPokemonEncounter *entry; ///< Holds the encounter
            Int32 index; ///< Holds the old encounter index
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
        UInt8 m_Probability;    ///< Encounter probability per step
        UInt32 m_Offset;        ///< Encounter array offset
        QList<WildPokemonEncounter *> m_Entries;
    };
}


#endif // __AME_WILDPOKEMONAREA_HPP__
