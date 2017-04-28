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
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <fstream>


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
    MapSortOrderType Settings::MapSortOrder;
    QString Settings::HexPrefix;
    QString Settings::LastPath;
    bool Settings::ShowRawMapHeader;
    bool Settings::ShowRawLayoutHeader;
    bool Settings::ShowGrid;
    int Settings::MapAccuracyLevel;
    QList<QString> Settings::RecentFiles;
	float Settings::ScaleFactor;


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Diegoisawesome, Pokedude, Nekaida
    // Last edit by:   Diegoisawesome
    // Date of edit:   4/21/2017
    //
    ///////////////////////////////////////////////////////////
    bool Settings::parse()
    {
        // Builds the path to the YAML file
        const QString appFolder = QApplication::applicationDirPath();
        const QString subFolder = "/config/";
        const QString fileName = "AME.yaml";
        QString filePath = appFolder + subFolder + fileName;

        // Loads the YAML file
        YAML::Node settings;

        QFileInfo check_file(filePath);
        if (check_file.exists() && check_file.isFile())
            settings = YAML::LoadFile(filePath.toStdString());

        if (settings.IsNull())
            Messages::showMessage(NULL, QObject::tr("AME configuration file (AME.yaml) not found.\nLoading default settings."));

        // Tries to parse all the properties
        ScaleFactor         = settings["ScaleFactor"].as<float>(1);
        ShowSprites         = settings["ShowSprites"].as<bool>(true);
        if (settings["ScriptEditor"].Type() != YAML::NodeType::Null)
            ScriptEditor    = QString::fromStdString(settings["ScriptEditor"].as<std::string>(""));
        Translucency        = settings["Translucency"].as<int>(50);
        if (settings["Language"].Type() != YAML::NodeType::Null)
            Language        = QString::fromStdString(settings["Language"].as<std::string>(""));
        CreateBackups       = settings["CreateBackups"].as<bool>(true);
        MapSortOrder        = static_cast<MapSortOrderType>(settings["MapSortOrder"].as<int>(0));
        if (settings["HexPrefix"].Type() != YAML::NodeType::Null)
            HexPrefix       = QString::fromStdString(settings["HexPrefix"].as<std::string>("0x"));
        ShowRawMapHeader    = settings["ShowRawMapHeader"].as<bool>(false);
        ShowRawLayoutHeader = settings["ShowRawLayoutHeader"].as<bool>(false);
        ShowGrid = settings["ShowGrid"].as<bool>(false);
        MapAccuracyLevel    = settings["MapAccuracyLevel"].as<int>(4);
        if (settings["LastPath"].Type() != YAML::NodeType::Null)
            LastPath        = QString::fromStdString(settings["LastPath"].as<std::string>(""));
        else
            LastPath        = QString();

        YAML::Node RecentFileNode = settings["RecentFiles"];
        int fileCount = RecentFileNode.size();
        if (fileCount > 10)
            fileCount = 10;
        for (int i = 0; i < fileCount; ++i) {
            RecentFiles.append(QString::fromStdString(RecentFileNode[i].as<std::string>("")));
        }

        // Parsing successful
        return true;
    }
    
    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   4/21/2017
    //
    ///////////////////////////////////////////////////////////
    bool Settings::write()
    {
        // Builds the path to the YAML file
        const QString appFolder = QApplication::applicationDirPath();
        const QString subFolder = "/config/";
        const QString fileName = "AME.yaml";
        QString filePath = appFolder + subFolder + fileName;

        if (!QDir().mkpath(appFolder + subFolder))
        {
            Messages::showError(NULL, QObject::tr("Error creating config folder.\nConfiguration could not be saved."));
            return false;
        }

        QFile file(filePath);
        if (!file.open(QIODevice::ReadWrite))
        {
            Messages::showError(NULL, QObject::tr("Error writing to configuration file AME.yaml.\nConfiguration could not be saved."));
            return false;
        }

        file.close();

        // Loads the YAML file
        YAML::Node settings = YAML::LoadFile(filePath.toStdString());

        // Tries to write all the properties
		settings["ScaleFactor"]         = ScaleFactor;
        settings["ShowSprites"]         = ShowSprites;
        settings["ScriptEditor"]        = ScriptEditor.toStdString();
        settings["Translucency"]        = Translucency;
        settings["Language"]            = Language.toStdString();
        settings["CreateBackups"]       = CreateBackups;
        settings["MapSortOrder"]        = static_cast<int>(MapSortOrder);
        settings["HexPrefix"]           = HexPrefix.toStdString();
        settings["ShowRawMapHeader"]    = ShowRawMapHeader;
        settings["ShowRawLayoutHeader"] = ShowRawLayoutHeader;
        settings["ShowGrid"]            = ShowGrid;
        settings["MapAccuracyLevel"]    = MapAccuracyLevel;
        settings["LastPath"]            = LastPath.toStdString();

        YAML::Node RecentFileNode;
        for(int i = 0; i < RecentFiles.count(); i++)
            RecentFileNode.push_back(RecentFiles[i].toStdString());

        settings["RecentFiles"]         = RecentFileNode;

        std::ofstream fout(filePath.toStdString());
        fout << settings;
        return true;
    }
}
