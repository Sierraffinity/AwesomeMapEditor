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
#include <AME/System/Settings.hpp>
#include <AME/Widgets/Misc/Messages.hpp>
#include <yaml-cpp/yaml.h>
#include <QApplication>
#include <QFile>
#include <QDebug>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Static variable definition
    //
    ///////////////////////////////////////////////////////////
    bool Settings::ShowSprites;
    QString Settings::ScriptEditor;
    int Settings::Translucency;
    QString Settings::Language;
    bool Settings::CreateBackups;
    int Settings::MapSortOrder;
    QString Settings::HexPrefix;
    bool Settings::ShowRawMapHeader;
    bool Settings::ShowRawLayoutHeader;


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Diegoisawesome, Pokedude
    // Last edit by:   Diegoisawesome
    // Date of edit:   6/29/2016
    //
    ///////////////////////////////////////////////////////////
    bool Settings::parse()
    {
        // Builds the path to the YAML file for this ROM
        const QString appFolder = QApplication::applicationDirPath();
        const QString subFolder = "/config/";
        const QString fileName = "AME.yaml";
        QString filePath = appFolder + subFolder + fileName;


        // Loads the YAML file
        YAML::Node settings = YAML::LoadFile(filePath.toStdString());
        if (settings.IsNull())
            Messages::showMessage(NULL, "Wherpsidingles");

        // Tries to parse all the properties
        ShowSprites         = settings["ShowSprites"].as<bool>();
        ScriptEditor        = QString::fromStdString(settings["ScriptEditor"].as<std::string>());
        Translucency        = settings["Translucency"].as<int>();
        Language            = QString::fromStdString(settings["Language"].as<std::string>());
        CreateBackups       = settings["CreateBackups"].as<bool>();
        MapSortOrder        = settings["MapSortOrder"].as<int>();
        HexPrefix           = QString::fromStdString(settings["HexPrefix"].as<std::string>());
        ShowRawMapHeader    = settings["ShowRawMapHeader"].as<bool>();
        ShowRawLayoutHeader = settings["ShowRawLayoutHeader"].as<bool>();

        // Parsing successful
        return true;
    }
}
