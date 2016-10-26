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


#ifndef __AME_MAINWINDOW_H__
#define __AME_MAINWINDOW_H__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/Mapping/Map.hpp>
#include <AME/Widgets/QFilterChildrenProxyModel.h>
#include <AME/Widgets/Listeners/MovePermissionListener.hpp>
#include <QMainWindow>
#include <QtWidgets>


namespace ame
{
    namespace Ui
    {
        class MainWindow;
    }


    ///////////////////////////////////////////////////////////
    /// \file    MainWindow.h
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/2/2016
    /// \brief   Holds components for the main AME window.
    ///
    ///////////////////////////////////////////////////////////
    class MainWindow : public QMainWindow {
    Q_OBJECT
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        /// \param parent Parental widget (optional)
        ///
        /// Creates all resources that are required by the GUI.
        ///
        ///////////////////////////////////////////////////////////
        explicit MainWindow(QWidget *parent = 0);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Releases all resources that belong to MainWindow.
        ///
        ///////////////////////////////////////////////////////////
        ~MainWindow();


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Cleans up OpenGL objects.
        ///
        /// This method is needed so the GL objects are destroyed
        /// with a context still being active.
        ///
        ///////////////////////////////////////////////////////////
        void closeEvent(QCloseEvent *event);

        ///////////////////////////////////////////////////////////
        /// \brief Shows a dialog to open a ROM file.
        ///
        /// Loads a ROM file and stores it in member m_Rom.
        /// Use the ErrorStack to retrieve errors that were thrown
        /// in this function.
        ///
        /// \returns true if no errors occured.
        ///
        ///////////////////////////////////////////////////////////
        bool openRomDialog();

        ///////////////////////////////////////////////////////////
        /// \brief Attempts to load all map-related data.
        ///
        /// Loads the configuration file and reads all data.
        /// Shows a window with error messages, in case one or
        /// more errors occured during the loading process.
        ///
        ///////////////////////////////////////////////////////////
        void loadMapData();

        ///////////////////////////////////////////////////////////
        /// \brief Sets the wild-pokemon tab up.
        ///
        ///////////////////////////////////////////////////////////
        void setupWildPokemon(Map *map);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the header tab up.
        ///
        ///////////////////////////////////////////////////////////
        void setupHeader(Map *map);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the GUI up after loading all map data.
        ///
        ///////////////////////////////////////////////////////////
        void setupAfterLoading();

        ///////////////////////////////////////////////////////////
        /// \brief Clears the map-related GUI things.
        ///
        ///////////////////////////////////////////////////////////
        void clearBeforeLoading();


    private slots:

        ///////////////////////////////////////////////////////////
        // Slots
        //
        ///////////////////////////////////////////////////////////
        void on_action_Open_ROM_triggered();
        void on_sldWildGrassChance_valueChanged(int value);
        void on_sldWildWaterChance_valueChanged(int value);
        void on_sldWildFishingChance_valueChanged(int value);
        void on_sldWildOtherChance_valueChanged(int value);
        void entity_mouseClick(QMouseEvent *event);
        void MapSortOrder_changed(QAction *action);
        void MapTabTool_changed(QAction *action);
        void RecentFile_triggered();
        void updateTreeView();
        void disableBeforeROMLoad();
        void enableAfterROMLoad();
        void enableAfterMapLoad();
        void loadRecentlyUsedFiles();
        void on_cmbEntityTypeSelector_currentIndexChanged(int index);
        void on_spnEntityScroller_valueChanged(int arg1);
        void on_cmbSignType_currentIndexChanged(int index);
        void showCorrectSignType(Sign *sign);
        void on_tabWidget_currentChanged(int index);
        void on_treeView_doubleClicked(const QModelIndex &index);
        void on_lineEdit_textChanged(const QString &arg1);
        bool loadROM(const QString &file);
        void on_actionClearRecentFiles_triggered();
        void on_tabWidget_2_currentChanged(int index);
        void on_action_Connection_Editor_triggered();
        void on_action_Settings_triggered();
        void on_action_Tileset_Editor_triggered();

    private:

        //////////////////////////////////////////////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        Ui::MainWindow *ui;                         ///< Gives access to the GUI objects
        qboy::Rom m_Rom;                            ///< Global ROM across the application
        QModelIndex *m_lastOpenedMap;               ///< Reference to the model index for the last opened map
        Map *m_CurrentMap;                          ///< Reference to currently opened map
        QLabel* statusLabel;                        ///< Status bar label reference
        QLabel* statusLabelCredit;                  ///< Status bar label reference for credit
        QFilterChildrenProxyModel *m_proxyModel;    ///< Tree view proxy model reference
        MovePermissionListener *m_MPListener;       ///< Move permission event listener
        
    };


    ///////////////////////////////////////////////////////////
    // Error messages for MainWindow
    //
    ///////////////////////////////////////////////////////////
    #define WND_ERROR_ROM   "The selected ROM file is already in use."
}


#endif // __AME_MAINWINDOW_H__
