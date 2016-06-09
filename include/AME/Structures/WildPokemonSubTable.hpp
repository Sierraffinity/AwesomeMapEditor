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


#ifndef __AME_WILDPOKEMONSUBTABLE_HPP__
#define __AME_WILDPOKEMONSUBTABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/System/IUndoable.hpp>
#include <AME/System/WriteEntry.hpp>
#include <AME/Structures/WildPokemonArea.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \brief Defines all existing encounter areas.
    ///
    ///////////////////////////////////////////////////////////
    enum EncounterArea
    {
        EA_AREA_GRASS   = 0,    ///< Encounters in high grass
        EA_AREA_WATER   = 1,    ///< Encounters while surfing
        EA_AREA_ROCK    = 2,    ///< Encounters on smashing rocks
        EA_AREA_FISH    = 3     ///< Encounters while fishing
    };


    ///////////////////////////////////////////////////////////
    /// \file    WildPokemonSubTable.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/3/2016
    /// \brief   Holds the encounters for multiple areas.
    ///
    /// Consists of four fixed-size arrays that contain the
    /// encounter entries for each of the areas.
    ///
    ///////////////////////////////////////////////////////////
    class WildPokemonSubTable {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::WildPokemonSubTable.
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonSubTable();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::WildPokemonSubTable.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonSubTable(const WildPokemonSubTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::WildPokemonSubTable
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonSubTable &operator=(const WildPokemonSubTable &rvalue);


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read all the area arrays.
        ///
        /// Each array has a fixed size and consists of several
        /// entries that define min/max level and Pokémon ID.
        ///
        /// \param rom Currently opened ROM file
        /// \param offset The offset of the sub table
        /// \returns true on success.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the area arrays to the same offset.
        ///
        /// As area arrays have a fixed size, they do not require
        /// a repoint.
        ///
        /// \returns a list of write entries to use for the rom.
        ///
        ///////////////////////////////////////////////////////////
        QList<WriteEntry> write();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the encounter array of one area.
        /// \param area Enum value specifying the returned area
        ///
        /// Valid values for the first argument are:
        /// EA_AREA_GRASS
        /// EA_AREA_WATER
        /// EA_AREA_ROCK
        /// EA_AREA_FISH
        ///
        ///////////////////////////////////////////////////////////
        WildPokemonArea &encounter(EncounterArea area);

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the offset of the sub-table.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 offset() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the associated map bank.
        ///
        ///////////////////////////////////////////////////////////
        UInt8 bank() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the associated map.
        ///
        ///////////////////////////////////////////////////////////
        UInt8 map() const;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt32 m_Offset;            ///< Offset of the sub-table
        UInt8 m_Bank;               ///< Associated map bank
        UInt8 m_Map;                ///< Associated map
        UInt32 m_PtrGrass;          ///< Offset of the grass array
        UInt32 m_PtrWater;          ///< Offset of the water array
        UInt32 m_PtrRock;           ///< Offset of the rock-smash array
        UInt32 m_PtrFish;           ///< Offset of the fishing array
        WildPokemonArea m_AreaGrass;
        WildPokemonArea m_AreaWater;
        WildPokemonArea m_AreaRock;
        WildPokemonArea m_AreaFish;
    };
}


#endif // __AME_WILDPOKEMONSUBTABLE_HPP__
