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


#ifndef __AME_MAP_HPP__
#define __AME_MAP_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Mapping/MapHeader.hpp>
#include <AME/Mapping/MappingErrors.hpp>
#include <AME/Entities/Tables/EventTable.hpp>
#include <AME/Entities/Tables/MapScriptTable.hpp>
#include <AME/Entities/Tables/ConnectionTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    Map.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/12/2016
    /// \brief   Contains the header, events and more.
    ///
    /// The map handles many other things, such as music,
    /// name string index, weather type, etc.
    ///
    ///////////////////////////////////////////////////////////
    class Map /* : IUndoable */ {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::Map.
        /// Note: Map is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because of the undo/redo system.
        ///
        ///////////////////////////////////////////////////////////
        Map();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::Map.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        Map(const Map &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::Map
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        Map &operator=(const Map &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated block objects.
        ///
        ///////////////////////////////////////////////////////////
        ~Map();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read the map.
        ///
        /// Reads the header, events, connections and properties.
        ///
        /// \param rom Currently opened ROM file
        /// \param offset Offset of the map
        /// \returns true if the map was read correctly.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a reference to the map's header.
        ///
        /// The map header is required to be modified outside this
        /// class to reduce complexity concerning undo/redo.
        ///
        /// \returns a reference to the map header.
        ///
        ///////////////////////////////////////////////////////////
        MapHeader &header();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a reference to the event table.
        ///
        /// The event table is required to be modified outside this
        /// class to reduce complexity concerning undo/redo.
        ///
        /// \returns a reference to the event table.
        ///
        ///////////////////////////////////////////////////////////
        EventTable &entities();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a reference to the connections.
        ///
        /// The connection table is required to be modified outside
        /// this class to reduce complexity concerning undo/redo.
        ///
        /// \returns a reference to the connection table.
        ///
        ///////////////////////////////////////////////////////////
        ConnectionTable &connections();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a reference to the mapscripts.
        ///
        /// The mapscript table is required to be modified outside
        /// this class to reduce complexity concerning undo/redo.
        ///
        /// \returns a reference to the mapscript table.
        ///
        ///////////////////////////////////////////////////////////
        MapScriptTable &scripts();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the name of the map in ASCII encoding.
        ///
        /// The name is needed for the tree-view containing all the
        /// maps.
        ///
        /// \returns a constant reference to the map string.
        ///
        ///////////////////////////////////////////////////////////
        const QString &name() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the index of the wild-pokemon table.
        ///
        /// The index specifies the position of the wild-pokemon
        /// table in the global wild-pokemon head table.
        ///
        /// \returns the position of the wild-pokemon table.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 wildpokeTable() const;


        ///////////////////////////////////////////////////////////
        /// \brief Specifies the wild-pokemon table.
        ///
        ///////////////////////////////////////////////////////////
        void setWildTable(UInt32 index);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt32 m_Offset;                ///< Offset of the map
        UInt32 m_PtrHeader;             ///< Offset of the header
        UInt32 m_PtrEvents;             ///< Offset of the entities
        UInt32 m_PtrScripts;            ///< Offset of the mapscripts
        UInt32 m_PtrConnections;        ///< Offset of the connections
        UInt16 m_MusicID;               ///< Index of the used soundtrack
        UInt16 m_HeaderID;              ///< Index in the footer table
        UInt8 m_NameIndex;              ///< Index in the name table
        UInt8 m_DarknessType;           ///< Darkness type (caves, forests)
        UInt8 m_WeatherType;            ///< Current weather on the map
        UInt8 m_MapType;                ///< Map type (indoor, outdoor)
        UInt8 m_LabelType;              ///< Label upon entering map
        UInt8 m_BattleType;             ///< Battle intro type
        MapHeader m_Header;             ///< Holds the map header properties
        EventTable m_Events;            ///< Holds all the entities
        MapScriptTable m_Scripts;       ///< Holds all level scripts
        ConnectionTable m_Connections;  ///< Holds all connections
        QString m_Name;                 ///< Holds the actual map name
        UInt32 m_WildTable;             ///< Holds the wildpokemon table index
    };
}


#endif // __AME_MAP_HPP__
