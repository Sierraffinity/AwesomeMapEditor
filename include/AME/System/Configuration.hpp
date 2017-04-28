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


#ifndef __AME_CONFIGURATION_HPP__
#define __AME_CONFIGURATION_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <QStringList>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    Configuration.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/16/2016
    /// \brief   Stores all YAML configuration values.
    ///
    /// Holds important offsets or values that can vary from
    /// game to game.
    ///
    ///////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////
    // Struct: BaseROMType
    //
    ///////////////////////////////////////////////////////////
    enum BaseROMType
    {
        RT_RS   =  0,
        RT_FRLG =  1,
        RT_EM   =  2
    };

    class Configuration {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Attempts to parse the configuration file.
        ///
        /// Some config values specify a pointer to a specific
        /// header; this function will retrieve the offset at
        /// these pointers.
        ///
        /// \param rom Used to fetch header offsets immediately
        /// \returns false if parsing failed.
        ///
        ///////////////////////////////////////////////////////////
        static bool parse(const qboy::Rom &rom);


        ///////////////////////////////////////////////////////////
        // Static class members
        //
        ///////////////////////////////////////////////////////////
        static BaseROMType RomType;
        static QString Name;
        static QString Language;
        static UInt32 PokemonCount;
        static UInt32 PokemonNames;
        static UInt32 PokemonIcons;
        static UInt32 PokemonUsage;
        static UInt32 PokemonPals;
        static UInt32 WildPokemon;
        static UInt32 ItemData;
        static UInt32 ItemCount;
        static UInt32 Overworlds;
        static UInt32 OverworldPals;
        static UInt32 OverworldCount;
        static UInt32 FlyingLocs;
        static UInt32 StartBank;
        static UInt32 StartMap;
        static UInt32 StartX;
        static UInt32 StartY;
        static UInt32 MapBanks;
        static UInt32 MapLayouts;
        static UInt32 MapNames;
        static UInt32 MapNameCount;
        static UInt32 MapNameTotal;
        static UInt32 SpriteNormalSet;
        static UInt32 SpriteSmallSet;
        static UInt32 SpriteLargeSet;
        static UInt32 WorldMapCount;
        static QList<UInt32> WorldMapGFX;
        static QList<UInt32> WorldMapTileMap;
        static QList<UInt32> WorldMapPal;
        static UInt32 FreespaceStart;
    };


    #define CONFIG(x)           (ame::Configuration::x)
    #define CFG_ERROR_FILE(x)   "YAML configuration file for ROM code '" + x + "' was not found."
    #define CFG_ERROR_OFFSET    "YAML configuration contained an invalid offset."
    #define FETCH(name, type)               \
    {                                       \
        if (!rom.seek(name))                \
            AME_THROW2(CFG_ERROR_OFFSET);   \
        name = type;                        \
    }
}


#endif // __AME_CONFIGURATION_HPP__
