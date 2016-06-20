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
#include <AME/System/Configuration.hpp>
#include <AME/Mapping/MappingErrors.hpp>
#include <AME/Mapping/Map.hpp>
#include <QBoy/Text/String.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    Map::Map()
        : /*IUndoable(),*/
          m_Offset(0),
          m_PtrHeader(0),
          m_PtrEvents(0),
          m_PtrScripts(0),
          m_PtrConnections(0),
          m_MusicID(0),
          m_HeaderID(0),
          m_NameIndex(0),
          m_DarknessType(0),
          m_WeatherType(0),
          m_MapType(0),
          m_LabelType(0),
          m_BattleType(0),
          m_WildTable(-1)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    Map::Map(const Map &rvalue)
        : /*IUndoable(),*/
          m_Offset(rvalue.m_Offset),
          m_PtrHeader(rvalue.m_PtrHeader),
          m_PtrEvents(rvalue.m_PtrEvents),
          m_PtrScripts(rvalue.m_PtrScripts),
          m_PtrConnections(rvalue.m_PtrConnections),
          m_MusicID(rvalue.m_MusicID),
          m_HeaderID(rvalue.m_HeaderID),
          m_NameIndex(rvalue.m_NameIndex),
          m_DarknessType(rvalue.m_DarknessType),
          m_WeatherType(rvalue.m_WeatherType),
          m_MapType(rvalue.m_MapType),
          m_LabelType(rvalue.m_LabelType),
          m_BattleType(rvalue.m_BattleType),
          m_Header(rvalue.m_Header),
          m_Events(rvalue.m_Events),
          m_Scripts(rvalue.m_Scripts),
          m_Connections(rvalue.m_Connections),
          m_Name(rvalue.m_Name),
          m_WildTable(rvalue.m_WildTable)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    Map &Map::operator=(const Map &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_PtrHeader = rvalue.m_PtrHeader;
        m_PtrEvents = rvalue.m_PtrEvents;
        m_PtrScripts = rvalue.m_PtrScripts;
        m_PtrConnections = rvalue.m_PtrConnections;
        m_MusicID = rvalue.m_MusicID;
        m_HeaderID = rvalue.m_HeaderID;
        m_NameIndex = rvalue.m_NameIndex;
        m_DarknessType = rvalue.m_DarknessType;
        m_WeatherType = rvalue.m_WeatherType;
        m_MapType = rvalue.m_MapType;
        m_LabelType = rvalue.m_LabelType;
        m_BattleType = rvalue.m_BattleType;
        m_Header = rvalue.m_Header;
        m_Events = rvalue.m_Events;
        m_Scripts = rvalue.m_Scripts;
        m_Connections = rvalue.m_Connections;
        m_Name = rvalue.m_Name;
        m_WildTable = rvalue.m_WildTable;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    Map::~Map()
    {
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    bool Map::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(MAP_ERROR_OFFSET, rom.redirected());

        // Reads all table pointers
        m_PtrHeader = rom.readPointerRef();
        m_PtrEvents = rom.readPointerRef();
        m_PtrScripts = rom.readPointerRef();
        m_PtrConnections = rom.readPointerRef();

        // Determines whether these offsets are valid
        if (!rom.checkOffset(m_PtrHeader))
            AME_THROW(MAP_ERROR_HEADER, offset);
        if (!rom.checkOffset(m_PtrEvents))
            AME_THROW(MAP_ERROR_EVENTS, offset + 4);
        if (!rom.checkOffset(m_PtrScripts))
            AME_THROW(MAP_ERROR_SCRIPTS, offset + 8);
        if (!rom.checkOffset(m_PtrConnections))
            AME_THROW(MAP_ERROR_CONNEX, offset + 12);


        // Attempts to read the header data
        if (!m_Header.read(rom, m_PtrHeader))
            return false;

        // Attempts to read the events
        if (m_PtrEvents)
            if (!m_Events.read(rom, m_PtrEvents))
                return false;

        // Attempts to read the map scripts
        if (m_PtrScripts)
            if (!m_Scripts.read(rom, m_PtrScripts))
                return false;

        // Attempts to read the map connections
        if (m_PtrConnections)
            if (!m_Connections.read(rom, m_PtrConnections))
                return false;


        // Reads all remaining map properties
        rom.seek(offset + 16);
        m_MusicID = rom.readHWord();
        m_HeaderID = rom.readHWord();
        m_NameIndex = rom.readByte();
        m_DarknessType = rom.readByte();
        m_WeatherType = rom.readByte();
        m_MapType = rom.readByte();
        rom.readHWord(); // padding
        m_LabelType = rom.readByte();
        m_BattleType = rom.readByte();


        // Determines the map name position
        if (rom.info().isFRLG())
            rom.seek(CONFIG(MapNames) + m_NameIndex*4 - 0x58*4);
        else
            rom.seek(CONFIG(MapNames) + m_NameIndex*8 + 4);

        unsigned ptrName = rom.readPointerRef();
        if (!rom.checkOffset(ptrName))
            AME_THROW(MAP_ERROR_NAME, rom.redirected());

        // Reads the map name string
        m_Name = qboy::String::read(rom, ptrName);
        m_Offset = offset;
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader &Map::header()
    {
        return m_Header;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    EventTable &Map::entities()
    {
        return m_Events;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    ConnectionTable &Map::connections()
    {
        return m_Connections;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    MapScriptTable &Map::scripts()
    {
        return m_Scripts;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    const QString &Map::name() const
    {
        return m_Name;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/12/2016
    //
    ///////////////////////////////////////////////////////////
    Int32 Map::wildpokeTable() const
    {
        return m_WildTable;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void Map::setWildTable(UInt32 index)
    {
        m_WildTable = index;
    }
}
