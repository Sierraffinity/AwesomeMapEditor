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


#ifndef __AME_SETTINGS_HPP__
#define __AME_SETTINGS_HPP__

///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QString>

namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    Settings.hpp
    /// \author  Diegoisawesome
    /// \version 1.0.0.0
    /// \date    6/29/2016
    /// \brief   Stores all AME settings.
    ///
    /// Holds global settings for use by AME.
    ///
    ///////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    // Struct: MapSortOrderType
    //
    ///////////////////////////////////////////////////////////
    enum MapSortOrderType
    {
        MSO_Name    =  0,
        MSO_Bank    =  1,
        MSO_Layout  =  2,
        MSO_Tileset =  3
    };

    class Settings {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Attempts to parse the settings file.
        ///
        /// \returns false if parsing failed.
        ///
        ///////////////////////////////////////////////////////////
        static bool parse();

        ///////////////////////////////////////////////////////////
        /// \brief Attempts to write the settings file.
        ///
        /// \returns false if writing failed.
        ///
        ///////////////////////////////////////////////////////////
        static bool write();

        ///////////////////////////////////////////////////////////
        // Static class members
        //
        ///////////////////////////////////////////////////////////
        static bool ShowSprites;
        static QString ScriptEditor;
        static int Translucency;
        static QString Language;
        static bool CreateBackups;
        static MapSortOrderType MapSortOrder;
        static QString HexPrefix;
        static QString LastPath;
        static bool ShowRawMapHeader;
        static bool ShowRawLayoutHeader;
        static bool ShowGrid;
        static int MapAccuracyLevel;
        static QList<QString> RecentFiles;
		static float ScaleFactor;
    };


    #define SETTINGS(x)           (ame::Settings::x)
    #define CHANGESETTING(x, val) (ame::Settings::x = val)
    #define SETG_ERROR_FILE      "YAML settings file could not be found."
	#define MAP_BLOCK_SIZE (SETTINGS(ScaleFactor) * 16)

}


#endif // __AME_SETTINGS_HPP__
