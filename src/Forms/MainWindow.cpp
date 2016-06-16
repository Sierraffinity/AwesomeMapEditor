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


#include <AME/System/LoadedData.hpp>
#include <AME/Widgets/Misc/Messages.hpp>
#include <AME/Forms/MainWindow.h>
#include "ui_MainWindow.h"
#include <QFileDialog>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/2/2016
    // Comment:
    //
    // Added a function call to set the GUI up.
    //
    ///////////////////////////////////////////////////////////
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/2/2016
    // Comment:
    //
    // Added a call to delete the GUI.
    //
    ///////////////////////////////////////////////////////////
    MainWindow::~MainWindow()
    {
        delete ui;
    }



    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/16/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::openRomDialog()
    {
        /* Replace QDir::homePath with SETTINGS(LastPath) */

        QString file = QFileDialog::getOpenFileName(
                    this,
                    QString("Open ROM file"),
                    QDir::homePath(),
                    QString("ROMs (*.gba *.bin)")
        );


        // Determines whether the dialog was successful
        if (!file.isNull() && !file.isEmpty())
        {
            // Close a previous ROM and destroy objects
            if (m_Rom.info().isLoaded())
            {
                // Clears the ROM data
                m_Rom.clearCache();
                m_Rom.close();

                // Clears old data and UI
                clearAllMapData();
                //clearBeforeLoading();
            }

            // Attempts to open the new ROM file
            if (!m_Rom.loadFromFile(file))
            {
                m_Rom.clearCache();
                m_Rom.close();

                Messages::showError(this, WND_ERROR_ROM);
                return false;
            }

            // Loading successful
            return true;
        }

        // Dialog cancelled
        return false;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/16/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::loadMapData()
    {
        /* TODO: Design form to show error messages */

        bool result = loadAllMapData(m_Rom);
        if (!result)
        {
            /* Show form */
        }

        QFile file("C:/Users/kogle/Desktop/file.txt");
        file.open(QIODevice::WriteOnly);
        file.write(ErrorStack::log().toStdString().c_str());
        file.flush();
        file.close();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/16/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_action_Open_ROM_triggered()
    {
        if (openRomDialog())
            loadMapData();
    }
}
