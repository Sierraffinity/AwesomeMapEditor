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
#include <AME/System/Configuration.hpp>
#include <AME/System/Settings.hpp>
#include <AME/Widgets/Misc/Messages.hpp>
#include <AME/Widgets/Rendering/AMEMapView.h>
#include <AME/Widgets/Rendering/AMEBlockView.h>
#include <AME/Forms/MainWindow.h>
#include <AME/Forms/ErrorWindow.h>
#include <AME/Forms/SettingsDialog.h>
#include <AME/Forms/TilesetDialog.h>
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QtEvents>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude, Diegoisawesome, Nekaida
    // Last edit by:   Diegoisawesome
    // Date of edit:   4/25/2017
    //
    ///////////////////////////////////////////////////////////
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_CurrentNPC(0),
        m_CurrentWarp(0),
        m_CurrentTrigger(0),
        m_CurrentSign(0),
        m_lastOpenedMap(NULL),
        m_MPListener(),
        m_statusLabel(tr("No ROM loaded.")),
        m_statusLabelCredit(tr("Created by ") + "<a href=\"http://domoreaweso.me/\">DoMoreAwesome</a>"),
        m_proxyModel(),
		m_CurrentMapManager()
    {
        // Setup GUI
        ui->setupUi(this);
        ui->lblMovementPerms->installEventFilter(&m_MPListener);



        connect(ui->glMapEditor, SIGNAL(loadMapChangeTreeView(Map*)), this, SLOT(loadMapChangeTreeView(Map*)));
        connect(ui->glEntityEditor, SIGNAL(loadMapChangeTreeView(Map*)), this, SLOT(loadMapChangeTreeView(Map*)));
        connect(ui->glEntityEditor, SIGNAL(onMouseClick(QMouseEvent*)), this, SLOT(entity_mouseClick(QMouseEvent*)));
        connect(ui->glEntityEditor, SIGNAL(onDoubleClick(QMouseEvent*)), this, SLOT(entity_doubleClick(QMouseEvent*)));

        ui->statusBar->addWidget(&m_statusLabel);

        m_statusLabelCredit.setTextFormat(Qt::RichText);
        m_statusLabelCredit.setTextInteractionFlags(Qt::TextBrowserInteraction);
        m_statusLabelCredit.setOpenExternalLinks(true);
        ui->statusBar->addPermanentWidget(&m_statusLabelCredit);

        QMenu *mapSortOrderMenu = new QMenu();
        QActionGroup *mapSortOrderActionGroup = new QActionGroup(this);

        mapSortOrderMenu->addAction(ui->actionSort_by_Name);
        mapSortOrderMenu->addAction(ui->actionSort_by_Bank);
        mapSortOrderMenu->addAction(ui->actionSort_by_Layout);
        mapSortOrderMenu->addAction(ui->actionSort_by_Tileset);
        ui->tbMapSortOrder->setMenu(mapSortOrderMenu);

        mapSortOrderActionGroup->addAction(ui->actionSort_by_Name);
        mapSortOrderActionGroup->addAction(ui->actionSort_by_Bank);
        mapSortOrderActionGroup->addAction(ui->actionSort_by_Layout);
        mapSortOrderActionGroup->addAction(ui->actionSort_by_Tileset);

        connect(ui->tbMapSortOrder, SIGNAL(triggered(QAction*)), this, SLOT(MapSortOrder_changed(QAction*)));

        QActionGroup *mapToolbarActionGroup = new QActionGroup(this);
        mapToolbarActionGroup->addAction(ui->actionMouse);
        ui->btnMapMouse->setDefaultAction(ui->actionMouse);
        mapToolbarActionGroup->addAction(ui->actionPencil);
        ui->btnMapPencil->setDefaultAction(ui->actionPencil);
        mapToolbarActionGroup->addAction(ui->actionEyedropper);
        ui->btnMapEyedropper->setDefaultAction(ui->actionEyedropper);
        mapToolbarActionGroup->addAction(ui->actionFill);
        ui->btnMapFill->setDefaultAction(ui->actionFill);
        mapToolbarActionGroup->addAction(ui->actionFillAll);
        ui->btnMapFillAll->setDefaultAction(ui->actionFillAll);

        connect(mapToolbarActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(MapTabTool_changed(QAction*)));

        /*QMenu *spriteModeMenu = new QMenu();
        QActionGroup *spriteModeActionGroup = new QActionGroup(this);

        spriteModeMenu->addAction(ui->actionSpriteAbove);
        spriteModeMenu->addAction(ui->actionSpriteTranslucent);
        spriteModeMenu->addAction(ui->actionSpriteBelow);
        spriteModeMenu->addAction(ui->actionSpriteBlocks);
        ui->tbSpriteMode->setMenu(spriteModeMenu);

        spriteModeActionGroup->addAction(ui->actionSpriteAbove);
        spriteModeActionGroup->addAction(ui->actionSpriteTranslucent);
        spriteModeActionGroup->addAction(ui->actionSpriteBelow);
        spriteModeActionGroup->addAction(ui->actionSpriteBlocks);

        connect(ui->tbSpriteMode, SIGNAL(triggered(QAction*)), this, SLOT(SpriteMode_changed(QAction*)));*/

        if (!Settings::parse())
            return;         // TODO: create default config file if none exists

        ui->glMapEditor->setBlockView(ui->glBlockEditor);
        ui->glMapEditor->setMPListener(&m_MPListener);
        ui->glMapEditor->setGridVisible(SETTINGS(ShowGrid));
        ui->glEntityEditor->setGridVisible(SETTINGS(ShowGrid));
        ui->glBlockEditor->setGridVisible(SETTINGS(ShowGrid));
        ui->btnShowSprites->setChecked(SETTINGS(ShowSprites));
        ui->btnMapGrid->setChecked(SETTINGS(ShowGrid));
        ui->btnEntitiesGrid->setChecked(SETTINGS(ShowGrid));
        disableBeforeROMLoad();

        if (SETTINGS(RecentFiles).count() > 0)
            loadRecentlyUsedFiles();
        else
            ui->actionRecent_Files->setEnabled(false);

        QAction* sortOrder = ui->tbMapSortOrder->menu()->actions()[SETTINGS(MapSortOrder)];
        ui->tbMapSortOrder->setIcon(sortOrder->icon());
        sortOrder->setChecked(true);

        /*QAction* spriteMode = ui->tbSpriteMode->menu()->actions()[SETTINGS(SpriteMode)];
        ui->tbSpriteMode->setIcon(spriteMode->icon());*/

        ui->treeView->setModel(&m_proxyModel);

        setScriptEditorButtonsEnabled(!SETTINGS(ScriptEditor).isEmpty());
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   4/25/2017
	//
    ///////////////////////////////////////////////////////////
    MainWindow::~MainWindow()
    {
        delete ui;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   4/26/2017
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::openRomDialog()
    {
        QString file = QFileDialog::getOpenFileName(
                    this,
                    tr("Open ROM File"),
                    SETTINGS(LastPath),
                    tr("ROMs (*.gba *.bin)")
        );

        // Determines whether the dialog was successful
        if (!file.isNull() && !file.isEmpty())
        {
            QFileInfo info(file);
            CHANGESETTING(LastPath, info.absolutePath());
            Settings::write();
            return loadROM(file);
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
    void MainWindow::clearBeforeLoading()
    {
        // First clears the tree-view
        //ui->treeView->collapseAll();
        //ui->treeView->clear();

        m_lastOpenedMap = NULL;
        m_CurrentMap = NULL;

        // Sets the tab index to the map-index
        ui->tabWidget->setCurrentIndex(0);
        ui->tabWidget->setEnabled(false);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::loadRecentlyUsedFiles()
    {
        QList<QString> recentFiles = SETTINGS(RecentFiles);
        ui->actionRecent_Files->setEnabled(true);
        QMenu *recentFilesMenu = new QMenu;
        for(int i = 0; i < recentFiles.count(); i++)
        {
            QAction *fileAction = new QAction(recentFilesMenu);
            fileAction->setText(QDir::toNativeSeparators(recentFiles[i]));
            fileAction->setData(recentFiles[i]);
            recentFilesMenu->addAction(fileAction);
            connect(fileAction, SIGNAL(triggered()), this, SLOT(RecentFile_triggered()));
        }
        recentFilesMenu->addSeparator();
        recentFilesMenu->addAction(ui->actionClearRecentFiles);
        ui->actionRecent_Files->setMenu(recentFilesMenu);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::loadROM(const QString &file)
    {
        // Close a previous ROM and destroy objects
        if (m_Rom.info().isLoaded())
        {
            // Clears the ROM data
            m_Rom.clearCache();
            m_Rom.close();

            // Clears old data and UI
            clearAllMapData();
            clearBeforeLoading();
        }

        // Attempts to open the new ROM file
        if (!m_Rom.loadFromFile(file))
        {
            m_Rom.clearCache();
            m_Rom.close();

            Messages::showError(this, m_Rom.lastError());
            return false;
        }

        // Add ROM file to recent files list
        QList<QString> recentFiles = SETTINGS(RecentFiles);

        int count = recentFiles.count();
        for (int i = 0; i < count; i++)
        {
            if (recentFiles[i] == file)
            {
                recentFiles.removeAt(i);
                i--;
                count--;
            }
        }

        recentFiles.prepend(file);
        if(recentFiles.count() > 10)
            recentFiles.removeLast();

        CHANGESETTING(RecentFiles, recentFiles);
        Settings::write();
        loadRecentlyUsedFiles();

        // Loading successful
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   3/1/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setScriptEditorButtonsEnabled(bool enabled)
    {
        ui->btnNPCOpenScript->setEnabled(enabled);
        ui->btnTriggerOpenScript->setEnabled(enabled);
        ui->btnSignOpenScript->setEnabled(enabled);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::loadMapData()
    {
        /* TODO: Design form to show error messages */

        int result = loadAllMapData(m_Rom);
        if (result < 0)
        {
            ErrorWindow errorWindow(this);
            errorWindow.exec();
            return;
        }

        //setWindowTitle(QString("Awesome Map Editor | %1").arg(m_Rom.info().name()));
		setWindowFilePath(m_Rom.info().path());
        m_statusLabel.setText(tr("ROM %1 loaded in %2 ms.").arg(m_Rom.info().name(), QString::number(result)));

        setupAfterLoading();
        m_Rom.clearCache();
        ui->glEntityEditor->setOverworlds(&dat_OverworldTable->argbImages());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude, Diegoisawesome, Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/30/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setupAfterLoading()
    {
        QStandardItemModel *pokemonModel = new QStandardItemModel;
        for (unsigned i = 0; i < CONFIG(PokemonCount); i++)
        {
            QStandardItem *item = new QStandardItem;
            item->setText(dat_PokemonTable->names().at(i));
            item->setIcon(QIcon(QPixmap::fromImage(dat_PokemonTable->images().at(i))));
            pokemonModel->appendRow(item);
        }

        // Fills all wild Pokemon comboboxes with the names
        foreach (QComboBox *box, ui->tabWidget_3->findChildren<QComboBox *>())
            box->setModel(pokemonModel);

        // Sets the max Pokemon IDs within the spinboxes
        foreach (QSpinBox *box, ui->tabWidget_3->findChildren<QSpinBox *>(QRegularExpression("sbWild")))
            box->setRange(0, CONFIG(PokemonCount) - 1);

        QStandardItemModel *itemModel = new QStandardItemModel;
        itemModel->appendRow(new QStandardItem(tr("Coins")));
        for (unsigned i = 1; i < CONFIG(ItemCount); i++)
        {
            QStandardItem *item = new QStandardItem;
            item->setText(dat_ItemTable->names().at(i));
            itemModel->appendRow(item);
        }

        // Fills all item comboboxes with the names
        ui->cmbSignItem->setModel(itemModel);

        // Sets the max item IDs within the spinboxes
        ui->spnSignItem->setRange(0, CONFIG(ItemCount) - 1);

        // Fills map name combobox in header view with names

        QStandardItemModel *mapNameModel = new QStandardItemModel;
        for (unsigned i = 0; i < CONFIG(MapNameCount); i++)
        {
            QStandardItem *item = new QStandardItem;
            item->setText(dat_MapNameTable->names()[i]->name);
            mapNameModel->appendRow(item);
        }

        ui->cmbHeaderMapName->setModel(mapNameModel);

        ui->header_mapname->setRange(CONFIG(MapNameTotal) - CONFIG(MapNameCount), CONFIG(MapNameTotal) - 1);

        // Updates the treeview
        updateTreeView();

        // Enables everything that should be on ROM load
        enableAfterROMLoad();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::disableBeforeROMLoad()
    {
        ui->centralWidget->setEnabled(false);
        ui->tabWidget->setEnabled(false);

        ui->action_Save_ROM->setEnabled(false);
        ui->action_Save_ROM_As->setEnabled(false);
        ui->action_Save_Map->setEnabled(false);
        ui->action_Reload_ROM->setEnabled(false);
        ui->action_Import->setEnabled(false);
        ui->action_Export->setEnabled(false);
        ui->action_Undo->setEnabled(false);
        ui->action_Redo->setEnabled(false);
        ui->action_World_Map_Editor->setEnabled(false);
        ui->action_Tileset_Editor->setEnabled(false);
        ui->action_Connection_Editor->setEnabled(false);
        ui->action_Show_Grid->setEnabled(false);

        ui->cbMapTimeOfDay->setVisible(false);
        ui->cbEntitiesTimeOfDay->setVisible(false);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::enableAfterROMLoad()
    {
        ui->centralWidget->setEnabled(true);
        ui->action_Save_ROM->setEnabled(true);
        ui->action_Save_ROM_As->setEnabled(true);
        ui->action_Reload_ROM->setEnabled(true);
        ui->action_World_Map_Editor->setEnabled(true);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::enableAfterMapLoad()
    {
        ui->tabWidget->setEnabled(true);

        ui->action_Save_Map->setEnabled(true);
        ui->action_Import->setEnabled(true);
        ui->action_Export->setEnabled(true);
        ui->action_Tileset_Editor->setEnabled(true);
        ui->action_Connection_Editor->setEnabled(true);
        ui->action_Show_Grid->setEnabled(true);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::updateTreeView()
    {
        // Fills the tree-view with all the maps
        //ui->treeView->collapseAll();
        ui->treeView->setUpdatesEnabled(false);

        // Creates icons for use in treeView
        QIcon mapFolderIcon;
        mapFolderIcon.addFile(QStringLiteral(":/icons/folder_closed_map.ico"), QSize(), QIcon::Normal, QIcon::Off);
        mapFolderIcon.addFile(QStringLiteral(":/icons/folder_map.ico"), QSize(), QIcon::Normal, QIcon::On);

        QIcon folderIcon;
        folderIcon.addFile(QStringLiteral(":/icons/folder_closed.ico"), QSize(), QIcon::Normal, QIcon::Off);

        QIcon mapIcon;
        mapIcon.addFile(QStringLiteral(":/icons/map.ico"), QSize(), QIcon::Normal, QIcon::Off);
        mapIcon.addFile(QStringLiteral(":/icons/image.ico"), QSize(), QIcon::Normal, QIcon::On);

        QStandardItemModel *standardModel = new QStandardItemModel;
        QStandardItem *root = standardModel->invisibleRootItem();

        switch(SETTINGS(MapSortOrder))
        {
            case MSO_Name:
            default:
            {
                // Adds every map name to the tree. Format: [<index>] <map name>
                int nameCount = CONFIG(MapNameCount);
                int nameTotal = CONFIG(MapNameTotal);
                for (int i = 0; i < nameCount; i++)
                {
                    QStandardItem *nameItem = new QStandardItem;
                    nameItem->setIcon(folderIcon);
                    nameItem->setEditable(false);

                    // Specifies the display name of the bank and adds it to the treeview
                    nameItem->setText('[' +
                                      QString("%1").arg(i + nameTotal - nameCount, 2 , 16, QChar('0')).toUpper() +
                                      "] " +
                                      dat_MapNameTable->names()[i]->name);
                    root->appendRow(nameItem);
                }
                int bankCount = dat_MapBankTable->banks().size();
                for (int i = 0; i < bankCount; i++)
                {
                    MapBank *bank = dat_MapBankTable->banks()[i];
                    // Adds all the bank's maps to the appropriate name
                    int mapCount = bank->maps().size();
                    for (int j = 0; j < mapCount; j++)
                    {
                        Map *map = bank->maps()[j];
                        int nameIndex = map->nameIndex() + nameCount - nameTotal;
                        QStandardItem *nameItem = root->child(nameIndex);
                        nameItem->setIcon(mapFolderIcon);

                        QStandardItem *mapItem = new QStandardItem();
                        mapItem->setIcon(mapIcon);
                        mapItem->setEditable(false);

                        mapItem->setText('[' +
                            QString("%1").arg(i, 2 , 16, QChar('0')).toUpper() +
                            ", " +
                            QString("%1").arg(j, 2 , 16, QChar('0')).toUpper() +
                            "] " +
                            dat_MapNameTable->names()[nameIndex]->name);

                        nameItem->appendRow(mapItem);

                        // Sets properties to identify map on click
                        /*QByteArray array;
                        array.append(i);
                        array.append(j);*/
                        mapItem->setData(QVariant::fromValue(map), Qt::UserRole);
                        map->setTreeViewIndex(mapItem->index());
                    }
                }
                break;
            }
            case MSO_Bank:
            {
                // Adds every bank and map to the tree. Format: [<bank>,<map>]
                int bankCount = dat_MapBankTable->banks().size();
                for (int i = 0; i < bankCount; i++)
                {
                    QStandardItem *bankItem = new QStandardItem;
                    MapBank *bank = dat_MapBankTable->banks()[i];
                    bankItem->setIcon(mapFolderIcon);
                    bankItem->setEditable(false);

                    // Adds all the bank's maps
                    int mapCount = bank->maps().size();
                    for (int j = 0; j < mapCount; j++)
                    {
                        QStandardItem *mapItem = new QStandardItem;
                        Map *map = bank->maps()[j];
                        mapItem->setIcon(mapIcon);
                        mapItem->setEditable(false);

                        mapItem->setText("[" +
                            QString("%1").arg(i, 2 , 16, QChar('0')).toUpper() +
                            ", " +
                            QString("%1").arg(j, 2 , 16, QChar('0')).toUpper() +
                            "] " +
                            dat_MapNameTable->names()[map->nameIndex() + CONFIG(MapNameCount) - CONFIG(MapNameTotal)]->name);

                        // Sets properties to identify map on click
                        /*QByteArray array;
                        array.append(i);
                        array.append(j);*/
                        mapItem->setData(QVariant::fromValue(map), Qt::UserRole);
                        map->setTreeViewIndex(mapItem->index());

                        // Adds the map to the bank
                        bankItem->appendRow(mapItem);
                    }

                    // Specifies the display name of the bank and adds it to the treeview
                    bankItem->setText('[' + QString("%1").arg(i, 2 , 16, QChar('0')).toUpper() + ']');
                    root->appendRow(bankItem);
                }
                break;
            }
            case MSO_Layout:
            {
                // Adds every layout index to the tree.
                int layoutTotal = dat_MapLayoutTable->count();
                for (int i = 1; i < layoutTotal; i++)
                {
                    QStandardItem *layoutItem = new QStandardItem;
                    layoutItem->setIcon(mapIcon);
                    layoutItem->setEditable(false);

                    // Specifies the display name of the bank and adds it to the treeview
                    layoutItem->setText('[' +
                                      QString("%1").arg(i, 4 , 16, QChar('0')).toUpper() +
                                      "] ");
                    layoutItem->setData(dat_MapLayoutTable->mapHeaders().at(i - 1)->offset(), Qt::UserRole);
                    root->appendRow(layoutItem);
                }
                int bankCount = dat_MapBankTable->banks().size();
                for (int i = 0; i < bankCount; i++)
                {
                    MapBank *bank = dat_MapBankTable->banks()[i];
                    // Adds all the bank's maps to the appropriate layout
                    int mapCount = bank->maps().size();
                    for (int j = 0; j < mapCount; j++)
                    {
                        Map *map = bank->maps()[j];
                        QStandardItem *layoutItem = root->child(map->layoutIndex() - 1);
                        layoutItem->setIcon(mapFolderIcon);

                        QStandardItem *mapItem = new QStandardItem();
                        mapItem->setIcon(mapIcon);
                        mapItem->setEditable(false);

                        mapItem->setText('[' +
                            QString("%1").arg(i, 2 , 16, QChar('0')).toUpper() +
                            ", " +
                            QString("%1").arg(j, 2 , 16, QChar('0')).toUpper() +
                            "] " +
                            dat_MapNameTable->names()[map->nameIndex() + CONFIG(MapNameCount) - CONFIG(MapNameTotal)]->name);

                        layoutItem->appendRow(mapItem);

                        // Sets properties to identify map on click
                        /*QByteArray array;
                        array.append(i);
                        array.append(j);*/
                        mapItem->setData(QVariant::fromValue(map), Qt::UserRole);
                        map->setTreeViewIndex(mapItem->index());
                    }
                }
                break;
            }
            case MSO_Tileset:
            {
                // Adds every tileset offset to the tree.
                int bankCount = dat_MapBankTable->banks().size();
                for (int i = 0; i < bankCount; i++)
                {
                    MapBank *bank = dat_MapBankTable->banks()[i];
                    // Adds all the bank's maps to the appropriate layout
                    int mapCount = bank->maps().size();
                    for (int j = 0; j < mapCount; j++)
                    {
                        Map *map = bank->maps()[j];
                        QModelIndexList primaryList;
                        if (root->rowCount() > 0)
                            primaryList = root->model()->match(root->child(0)->index(), Qt::UserRole, map->header().ptrPrimary());
                        QModelIndexList secondaryList;
                        if (root->rowCount() > 0)
                            secondaryList = root->model()->match(root->child(0)->index(), Qt::UserRole, map->header().ptrSecondary());

                        QStandardItem *primaryItem;
                        QStandardItem *secondaryItem;

                        if (primaryList.count() <= 0 || !primaryList.first().isValid())
                        {
                            primaryItem = new QStandardItem();
                            primaryItem->setEditable(false);
                            primaryItem->setText('[' +
                                              QString("%1").arg(map->header().ptrPrimary(), 8 , 16, QChar('0')).toUpper() +
                                              "] ");
                            primaryItem->setData(map->header().ptrPrimary(), Qt::UserRole);
                            root->appendRow(primaryItem);
                        }
                        else
                            primaryItem = static_cast<QStandardItemModel*>(root->model())->itemFromIndex(primaryList.first());

                        if (secondaryList.count() <= 0 || !secondaryList.first().isValid())
                        {
                            secondaryItem = new QStandardItem();
                            secondaryItem->setEditable(false);
                            secondaryItem->setText('[' +
                                              QString("%1").arg(map->header().ptrSecondary(), 8 , 16, QChar('0')).toUpper() +
                                              "] ");
                            secondaryItem->setData(map->header().ptrSecondary(), Qt::UserRole);
                            root->appendRow(secondaryItem);
                        }
                        else
                            secondaryItem = static_cast<QStandardItemModel*>(root->model())->itemFromIndex(secondaryList.first());

                        primaryItem->setIcon(mapFolderIcon);
                        secondaryItem->setIcon(mapFolderIcon);
\
                        QStandardItem *mapItem = new QStandardItem();
                        mapItem->setIcon(mapIcon);
                        mapItem->setEditable(false);

                        mapItem->setText('[' +
                            QString("%1").arg(i, 2 , 16, QChar('0')).toUpper() +
                            ", " +
                            QString("%1").arg(j, 2 , 16, QChar('0')).toUpper() +
                            "] " +
                            dat_MapNameTable->names()[map->nameIndex() + CONFIG(MapNameCount) - CONFIG(MapNameTotal)]->name);

                        // Sets properties to identify map on click
                        /*QByteArray array;
                        array.append(i);
                        array.append(j);*/
                        mapItem->setData(QVariant::fromValue(map), Qt::UserRole);
                        map->setTreeViewIndex(mapItem->index());

                        primaryItem->appendRow(mapItem);
                        secondaryItem->appendRow(mapItem->clone());
                    }
                }
                m_proxyModel.sort(0, Qt::AscendingOrder);
                break;
            }
        }
        m_proxyModel.setSourceModel(standardModel);

        // Repaint tree-view
        ui->treeView->setUpdatesEnabled(true);
        ui->treeView->header()->setDefaultSectionSize(600);
        ui->treeView->repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setupWildPokemon(Map *map)
    {
        ui->tabWidget_3->setCurrentIndex(0);
        ui->tabWidget_3->setTabEnabled(0, false);
        ui->tabWidget_3->setTabEnabled(1, false);
        ui->tabWidget_3->setTabEnabled(2, false);
        ui->tabWidget_3->setTabEnabled(3, false);


        if (map->wildpokeTable() == -1)
            return;

        WildPokemonSubTable *table = dat_WildPokemonTable->tables()[map->wildpokeTable()];
        WildPokemonArea &grass = table->encounter(EA_AREA_GRASS);
        WildPokemonArea &water = table->encounter(EA_AREA_WATER);
        WildPokemonArea &fish = table->encounter(EA_AREA_FISH);
        WildPokemonArea &rock = table->encounter(EA_AREA_ROCK);


        // Fills the encounter probabilities
        ui->sbWildGrassChance->setValue(grass.probability());
        ui->sbWildWaterChance->setValue(water.probability());
        ui->sbWildFishingChance->setValue(fish.probability());
        ui->sbWildOtherChance->setValue(rock.probability());
        ui->tabWidget_3->setCurrentIndex(0);

        // Fills the pokemon ID and min/max level
        for (int i = 0; i < grass.entries().size(); i++)
        {
            QString num = QString::number(i+1);
            WildPokemonEncounter *pkm = grass.entries().at(i);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWildGrass") + num)
                    ->setValue(pkm->id);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbGrassMin") + num)
                    ->setValue(pkm->min);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbGrassMax") + num)
                    ->setValue(pkm->max);
        }
        for (int i = 0; i < water.entries().size(); i++)
        {
            QString num = QString::number(i+1);
            WildPokemonEncounter *pkm = water.entries().at(i);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWildWater") + num)
                    ->setValue(pkm->id);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWaterMin") + num)
                    ->setValue(pkm->min);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWaterMax") + num)
                    ->setValue(pkm->max);
        }
        for (int i = 0; i < fish.entries().size(); i++)
        {
            QString num = QString::number(i+1);
            WildPokemonEncounter *pkm = fish.entries().at(i);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWildFishing") + num)
                    ->setValue(pkm->id);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbFishingMin") + num)
                    ->setValue(pkm->min);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbFishingMax") + num)
                    ->setValue(pkm->max);
        }
        for (int i = 0; i < rock.entries().size(); i++)
        {
            QString num = QString::number(i+1);
            WildPokemonEncounter *pkm = rock.entries().at(i);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbWildOther") + num)
                    ->setValue(pkm->id);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbOtherMin") + num)
                    ->setValue(pkm->min);

            ui->tabWidget_3
                    ->findChild<QSpinBox *>(QString("sbOtherMax") + num)
                    ->setValue(pkm->max);
        }


        // Enables the tabs, depending on which wild-pokemon areas exist
        if (grass.entries().size() > 0)
            ui->tabWidget_3->setTabEnabled(0, true);
        if (water.entries().size() > 0)
            ui->tabWidget_3->setTabEnabled(1, true);
        if (fish.entries().size() > 0)
            ui->tabWidget_3->setTabEnabled(2, true);
        if (rock.entries().size() > 0)
            ui->tabWidget_3->setTabEnabled(3, true);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setupHeader(Map *map)
    {
        // Loads all the map-specific data
        ui->header_ptr_footer->setValue(map->m_PtrHeader);
        ui->header_ptr_script->setValue(map->m_PtrScripts);
        ui->header_ptr_event->setValue(map->m_PtrEvents);
        ui->header_ptr_connex->setValue(map->m_PtrConnections);

        ui->cmbHeaderMapName->setCurrentIndex(map->m_NameIndex - CONFIG(MapNameTotal) + CONFIG(MapNameCount));
        ui->header_mapname->setValue(map->m_NameIndex);
        ui->header_ftr_index->setValue(map->m_HeaderID);
        ui->header_music_index->setValue(map->m_MusicID);
        ui->cbVisibility->setCurrentIndex(map->m_DarknessType);
        ui->cbMapType->setCurrentIndex(map->m_MapType);
        ui->cbWeather->setCurrentIndex(map->m_WeatherType);
        ui->cbBattleBG->setCurrentIndex(map->m_BattleType);

        if (CONFIG(RomType) == RT_FRLG)
        {
            ui->header_check_showname->setChecked(map->m_MiscByte2 & 4);
            ui->header_check_run->setChecked(map->m_MiscByte2 & 2);
            ui->header_check_bike->setChecked(map->m_MiscByte1 & 1);
            ui->header_check_escape->setChecked(map->m_MiscByte2 & 1);
        }
        else if (CONFIG(RomType) == RT_EM)
        {
            ui->header_check_showname->setChecked(map->m_MiscByte3 & 8);
            ui->header_check_run->setChecked(map->m_MiscByte3 & 4);
            ui->header_check_bike->setChecked(map->m_MiscByte3 & 1);
            ui->header_check_escape->setChecked(map->m_MiscByte3 & 2);
        }

        ui->header_raw_data->setData(map->rawData());
        ui->header_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(map->m_Offset, 16).toUpper());


        // Loads all the footer-specific data
        MapHeader &header = map->header();
        ui->header_ftr_ptr_border->setValue(header.m_PtrBorder);
        ui->header_ftr_ptr_data->setValue(header.m_PtrBlocks);
        ui->header_ftr_ptr_global->setValue(header.m_PtrPrimary);
        ui->header_ftr_ptr_local->setValue(header.m_PtrSecondary);
        ui->header_map_width->setText(QString::number(header.size().width()));
        ui->header_map_height->setText(QString::number(header.size().height()));
        ui->header_border_width->setText(QString::number(header.border().width()));
        ui->header_border_height->setText(QString::number(header.border().height()));
        ui->header_ftr_raw_data->setData(header.rawData());
        ui->header_ftr_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(header.m_Offset, 16).toUpper());

        if (CONFIG(RomType) != RT_FRLG)
        {
            ui->header_border_width->setEnabled(false);
            ui->header_border_height->setEnabled(false);
        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Slot
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

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_sldWildGrassChance_valueChanged(int value)
    {
        ui->lblWildGrassChance->setText(QString::number((int)((value / 255.0)*100)) + QString("%"));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_sldWildWaterChance_valueChanged(int value)
    {
        ui->lblWildWaterChance->setText(QString::number((int)((value / 255.0)*100)) + QString("%"));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_sldWildFishingChance_valueChanged(int value)
    {
        ui->lblWildFishingChance->setText(QString::number((int)((value / 255.0)*100)) + QString("%"));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_sldWildOtherChance_valueChanged(int value)
    {
        ui->lblWildOtherChance->setText(QString::number((int)((value / 255.0)*100)) + QString("%"));
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Pokedude
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_treeView_activated(const QModelIndex &index)
    {
        if (!index.parent().isValid())
        {
            if (!index.child(0, 0).isValid() && SETTINGS(MapSortOrder) == MSO_Layout)
            {
                // Switch opened map icon
                if(m_lastOpenedMap != NULL)
                {
                    ui->treeView->setExpanded(*m_lastOpenedMap, false);
                    delete m_lastOpenedMap;
                }

                ui->treeView->setExpanded(index, true);
                m_lastOpenedMap = new QModelIndex(index);

                // Populates the map tab
                UInt32 offset = ui->treeView->model()->data(index, Qt::UserRole).toUInt();
                if (offset == 0)
                {
                    // Map layout is NULL pointer, abort
                    Messages::showMessage(this, "Layout says: \"I don't exist! *cries*\"");
                    return;
                }

                MapHeader header;
                header.read(m_Rom, offset);
                if (header.primary()->image()->raw().isEmpty() ||
                    header.primary()->image()->raw().isEmpty())
                {
                    // Tilesets invalid, abort
                    Messages::showMessage(this, "Layout says: \"I don't want to load tilesets!\"");
                    return;
                }


                ui->glMapEditor->setLayout(header);
                ui->glMapEditor->update();
                ui->glBlockEditor->setMapView(ui->glMapEditor);
                ui->glBlockEditor->update();
                ui->glBorderEditor->setMapView(ui->glMapEditor, true);
                ui->glBorderEditor->update();

                // Sets up the header, invisibles
                ui->tabWidget->setTabEnabled(1, false);
                ui->tabWidget->setTabEnabled(2, false);
                //setupHeader(currentMap);
                enableAfterMapLoad();
            }
            return;
        }

        changeTreeViewMap(index);

        // Retrieves the new map from the stored property
        //QByteArray data = ui->treeView->model()->data(index, Qt::UserRole).toByteArray();

        loadMap(qvariant_cast<Map *>(ui->treeView->model()->data(index, Qt::UserRole)), ui->treeView->model()->data(index, Qt::DisplayRole).toString());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::changeTreeViewMap(const QModelIndex &index)
    {
        if(m_lastOpenedMap != NULL)
        {
            ui->treeView->setExpanded(m_proxyModel.mapFromSource(*m_lastOpenedMap), false);
            delete m_lastOpenedMap;
        }

        ui->treeView->scrollTo(index);
        ui->treeView->setExpanded(index, true);
        m_lastOpenedMap = new QModelIndex(m_proxyModel.mapToSource(index));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::loadMapChangeTreeView(int bank, int map)
    {
        Map *currentMap = dat_MapBankTable->banks()[bank]->maps()[map];
        return loadMapChangeTreeView(currentMap);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::loadMapChangeTreeView(Map *map)
    {
        const QModelIndex index = m_proxyModel.mapFromSource(*map->getTreeViewIndex());
        changeTreeViewMap(index);

        return loadMap(map, index.data(Qt::DisplayRole).toString());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::loadMap(int bank, int map)
    {
        return loadMap(bank, map, NULL);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::loadMap(int bank, int map, const QString &name)
    {
        Map *currentMap = dat_MapBankTable->banks()[bank]->maps()[map];
        return loadMap(currentMap, name);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::loadMap(Map *currentMap)
    {
        return loadMap(currentMap, NULL);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::loadMap(Map *currentMap, const QString &name)
    {
        // Counts how long it takes to load a map
        QTime stopWatch;
        stopWatch.start();

        // Fills all the OpenGL widgets
        ui->glMapEditor->setMap(m_Rom, currentMap);
        ui->glMapEditor->update();
        ui->glBlockEditor->setMapView(ui->glMapEditor);
        ui->glBlockEditor->update();
        ui->glBorderEditor->setMapView(ui->glMapEditor);
        ui->glBorderEditor->update();
        ui->glEntityEditor->setMapView(ui->glMapEditor);
        ui->glEntityEditor->setEntities(currentMap);
        m_CurrentMap = currentMap;

        // Populates the wild Pokémon tab
        setupWildPokemon(currentMap);

        // Populates the header tab
        setupHeader(currentMap);
        //ui->cmbEntityTypeSelector->setCurrentIndex(0);
        on_cmbEntityTypeSelector_currentIndexChanged(ui->cmbEntityTypeSelector->currentIndex());

        if (name != NULL)
        {
            m_statusLabel.setText(tr("Map %1 loaded in %2 ms.").arg(name, QString::number(stopWatch.elapsed())));
            //setWindowTitle(QString("Awesome Map Editor | %1 | %2").arg(m_Rom.info().name(), name));
			//setWindowTitle(QString("%1 - %2").arg(name, m_Rom.info().name()));
        }
        else
        {
            m_statusLabel.setText(tr("Map loaded in %2 ms.").arg(QString::number(stopWatch.elapsed())));
            //setWindowTitle(QString("Awesome Map Editor | %1").arg(m_Rom.info().name()));
			//setWindowTitle(m_Rom.info().name());
        }

        // FIX: Scroll to main map
        QTimer::singleShot
        (
            10, Qt::PreciseTimer, [this] ()
            {
                QPoint scrollPos = ui->glMapEditor->mainPos();
                QSize size = ui->glMapEditor->mainMap()->header().size();

                int xMap = (ui->scrollArea->viewport()->width() - size.width() * 16) / 2;
                int yMap = (ui->scrollArea->viewport()->height() - size.height() * 16) / 2;
                ui->scrollArea->horizontalScrollBar()->setValue(scrollPos.x() - ((xMap > 0) ? xMap : 0));
                ui->scrollArea->verticalScrollBar()->setValue(scrollPos.y() - ((yMap > 0) ? yMap : 0));

                int xEnt = (ui->scrollArea_5->viewport()->width() - size.width() * 16) / 2;
                int yEnt = (ui->scrollArea_5->viewport()->height() - size.height() * 16) / 2;
                ui->scrollArea_5->horizontalScrollBar()->setValue(scrollPos.x() - ((xEnt > 0) ? xEnt : 0));
                ui->scrollArea_5->verticalScrollBar()->setValue(scrollPos.y() - ((yEnt > 0) ? yEnt : 0));

                if (ui->tabWidget->currentIndex() == 0)
                {
                    QMouseEvent eve( (QEvent::MouseMove), ui->glMapEditor->mapFromGlobal(QCursor::pos()),
                                     Qt::NoButton,
                                     Qt::NoButton,
                                     Qt::NoModifier   );
                    QApplication::sendEvent(ui->glMapEditor, &eve);
                }
                else if (ui->tabWidget->currentIndex() == 1)
                {
                    QMouseEvent eve( (QEvent::MouseMove), ui->glEntityEditor->mapFromGlobal(QCursor::pos()),
                                     Qt::NoButton,
                                     Qt::NoButton,
                                     Qt::NoModifier   );
                    QApplication::sendEvent(ui->glEntityEditor, &eve);
                }
            }
        );

        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setTabEnabled(2, true);
        enableAfterMapLoad();

        return true;
    }



    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::MapSortOrder_changed(QAction *action)
    {
        ui->tbMapSortOrder->setIcon(action->icon());
        QList<QAction*> items = ui->tbMapSortOrder->menu()->actions();
        int index = 0;
        for (int i = 0; i < items.count(); i++)
        {
            if(items[i] == action)
            {
                index = i;
                break;
            }
        }
        CHANGESETTING(MapSortOrder, static_cast<MapSortOrderType>(index));
        Settings::write();
        if(m_Rom.info().isLoaded())
            updateTreeView();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/26/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::MapTabTool_changed(QAction *action)
    {
        if (action == ui->actionPencil)
        {
            ui->glMapEditor->setCurrentTool(AMEMapView::Tool::Draw);
        }
        else if (action == ui->actionEyedropper)
        {
            ui->glMapEditor->setCurrentTool(AMEMapView::Tool::Select);
        }
        else if (action == ui->actionFill)
        {
            ui->glMapEditor->setCurrentTool(AMEMapView::Tool::Fill);
        }
        else if (action == ui->actionFillAll)
        {
            ui->glMapEditor->setCurrentTool(AMEMapView::Tool::FillAll);
        }
        else
        {
            ui->glMapEditor->setCurrentTool(AMEMapView::Tool::None);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_btnShowSprites_toggled(bool checked)
    {
        CHANGESETTING(ShowSprites, checked);
        Settings::write();
        if(m_Rom.info().isLoaded() && ui->tabWidget->currentIndex() == 1)
            ui->glEntityEditor->repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/26/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_btnMapGrid_toggled(bool checked)
    {
        toggle_grid(checked);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/26/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_btnEntitiesGrid_toggled(bool checked)
    {
        toggle_grid(checked);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_cmbEntityTypeSelector_currentIndexChanged(int index)
    {
        if (index == 0)
        {
            ui->spnEntityScroller->setMinimum(0);
            ui->spnEntityScroller->setMaximum(m_CurrentMap->entities().npcs().size()-1);
            ui->stckEntityEditor->setEnabled(m_CurrentMap->entities().npcs().size() > 0);

            if (m_CurrentMap->entities().npcs().size() > 0)
                on_spnEntityScroller_valueChanged(m_CurrentNPC);
            else
                ui->glEntityEditor->setCurrentEntity(CurrentEntity());
        }
        else if (index == 1)
        {
            ui->spnEntityScroller->setMinimum(0);
            ui->spnEntityScroller->setMaximum(m_CurrentMap->entities().warps().size()-1);
            ui->stckEntityEditor->setEnabled(m_CurrentMap->entities().warps().size() > 0);

            if (m_CurrentMap->entities().warps().size() > 0)
                on_spnEntityScroller_valueChanged(m_CurrentWarp);
            else
                ui->glEntityEditor->setCurrentEntity(CurrentEntity());
        }
        else if (index == 2)
        {
            ui->spnEntityScroller->setMinimum(0);
            ui->spnEntityScroller->setMaximum(m_CurrentMap->entities().triggers().size()-1);
            ui->stckEntityEditor->setEnabled(m_CurrentMap->entities().triggers().size() > 0);

            if (m_CurrentMap->entities().triggers().size() > 0)
                on_spnEntityScroller_valueChanged(m_CurrentTrigger);
            else
                ui->glEntityEditor->setCurrentEntity(CurrentEntity());
        }
        else
        {
            ui->spnEntityScroller->setMinimum(0);
            ui->spnEntityScroller->setMaximum(m_CurrentMap->entities().signs().size()-1);
            ui->stckEntityEditor->setEnabled(m_CurrentMap->entities().signs().size() > 0);

            if (m_CurrentMap->entities().signs().size() > 0)
                on_spnEntityScroller_valueChanged(m_CurrentSign);
            else
                ui->glEntityEditor->setCurrentEntity(CurrentEntity());
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude, Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/27/2017
    //
    /////////////////////////////////////////////////////////
    void MainWindow::on_spnEntityScroller_valueChanged(int arg1)
    {
        if (arg1 < 0)
            return;


        QPoint startPos = ui->glMapEditor->mainPos();
        if (ui->cmbEntityTypeSelector->currentIndex() == 0 && m_CurrentMap->entities().npcs().size() > 0)
        {
            if (arg1 >= m_CurrentMap->entities().npcs().size())
                m_CurrentNPC = m_CurrentMap->entities().npcs().size() - 1;
            else
                m_CurrentNPC = arg1;
            Npc *eventN = m_CurrentMap->entities().npcs()[m_CurrentNPC];
            ui->stckEntityEditor->setCurrentWidget(ui->pageNPCs);
            ui->npc_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(eventN->offset, 16).toUpper());
            ui->npc_num->setValue(eventN->npcID);
            ui->npc_sprite->setValue(eventN->imageID);
            ui->npc_pos_x->setValue(eventN->positionX);
            ui->npc_pos_y->setValue(eventN->positionY);
            ui->spnNPCHeight->setValue(eventN->level);
            ui->npc_replacement->setValue(eventN->replacement);
            ui->spnNPCIdleAnim->setValue(eventN->behaviour);
            ui->npc_mov_x->setValue(eventN->moveRadiusX);
            ui->npc_mov_y->setValue(eventN->moveRadiusY);
            ui->npc_view_rad->setValue(eventN->viewRadius);
            ui->npc_trainer->setValue(eventN->property);
            ui->npc_script->setValue(eventN->ptrScript);
            ui->npc_flag->setValue(eventN->flag);
            ui->npc_raw_data->setData(eventN->rawData());
            ui->spnEntityScroller->setValue(m_CurrentNPC);
            startPos += QPoint(eventN->positionX*16, eventN->positionY*16);

            CurrentEntity entity;
            entity.absPos = startPos;
            entity.type = ET_Npc;
            entity.entity = eventN;
            entity.index = m_CurrentNPC;
            ui->glEntityEditor->setCurrentEntity(entity);
        }
        else if (ui->cmbEntityTypeSelector->currentIndex() == 1 && m_CurrentMap->entities().warps().size() > 0)
        {
            if (arg1 >= m_CurrentMap->entities().warps().size())
                m_CurrentWarp = m_CurrentMap->entities().warps().size() - 1;
            else
                m_CurrentWarp = arg1;
            Warp *eventW = m_CurrentMap->entities().warps()[m_CurrentWarp];
            ui->stckEntityEditor->setCurrentWidget(ui->pageWarps);
            ui->warp_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(eventW->offset, 16).toUpper());
            ui->warp_pos_x->setValue(eventW->positionX);
            ui->warp_pos_y->setValue(eventW->positionY);
            ui->warp_number->setValue(eventW->warp);
            ui->warp_map->setValue(eventW->map);
            ui->warp_bank->setValue(eventW->bank);
            ui->spnWarpHeight->setValue(eventW->level);
            ui->warp_raw_data->setData(eventW->rawData());
            ui->spnEntityScroller->setValue(m_CurrentWarp);
            startPos += QPoint(eventW->positionX*16, eventW->positionY*16);

            checkWarp();

            CurrentEntity entity;
            entity.absPos = startPos;
            entity.type = ET_Warp;
            entity.entity = eventW;
            entity.index = m_CurrentWarp;
            ui->glEntityEditor->setCurrentEntity(entity);
        }
        else if (ui->cmbEntityTypeSelector->currentIndex() == 2 && m_CurrentMap->entities().triggers().size() > 0)
        {
            if (arg1 >= m_CurrentMap->entities().triggers().size())
                m_CurrentTrigger = m_CurrentMap->entities().triggers().size() - 1;
            else
                m_CurrentTrigger = arg1;
            Trigger *eventT = m_CurrentMap->entities().triggers()[m_CurrentTrigger];
            ui->stckEntityEditor->setCurrentWidget(ui->pageTriggers);
            ui->trigger_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(eventT->offset, 16).toUpper());
            ui->trigger_pos_x->setValue(eventT->positionX);
            ui->trigger_pos_y->setValue(eventT->positionY);
            ui->trigger_var->setValue(eventT->variable);
            ui->trigger_value->setValue(eventT->value);
            ui->trigger_script->setValue(eventT->ptrScript);
            ui->spnTriggerHeight->setValue(eventT->level);
            ui->trigger_raw_data->setData(eventT->rawData());
            ui->spnEntityScroller->setValue(m_CurrentTrigger);
            startPos += QPoint(eventT->positionX*16, eventT->positionY*16);

            CurrentEntity entity;
            entity.absPos = startPos;
            entity.type = ET_Trigger;
            entity.entity = eventT;
            entity.index = m_CurrentTrigger;
            ui->glEntityEditor->setCurrentEntity(entity);
        }
        else if (m_CurrentMap->entities().signs().size() > 0)
        {
            if (arg1 >= m_CurrentMap->entities().signs().size())
                m_CurrentSign = m_CurrentMap->entities().signs().size() - 1;
            else
                m_CurrentSign = arg1;
            Sign *eventS = m_CurrentMap->entities().signs()[m_CurrentSign];
            ui->stckEntityEditor->setCurrentWidget(ui->pageSigns);
            ui->sign_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(eventS->offset, 16).toUpper());
            ui->sign_pos_x->setValue(eventS->positionX);
            ui->sign_pos_y->setValue(eventS->positionY);
            ui->sign_script->setValue(eventS->ptrScript);
            ui->sign_raw_data->setData(eventS->rawData());
            ui->spnSignHeight->setValue(eventS->level);
            ui->spnSignType->setValue(static_cast<int>(eventS->type));
            ui->spnEntityScroller->setValue(m_CurrentSign);
            startPos += QPoint(eventS->positionX*16, eventS->positionY*16);

            CurrentEntity entity;
            entity.absPos = startPos;
            entity.type = ET_Sign;
            entity.entity = eventS;
            entity.index = m_CurrentSign;
            ui->glEntityEditor->setCurrentEntity(entity);

            showCorrectSignType(eventS);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_cmbSignType_currentIndexChanged(int index)
    {
        Q_UNUSED(index);
        Sign *eventS = m_CurrentMap->entities().signs()[ui->spnEntityScroller->value()];
        showCorrectSignType(eventS);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude, Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/27/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::entity_mouseClick(QMouseEvent *event)
    {
        // Map coordinates to block-coordinates
        int blockX = (event->pos().x()-ui->glMapEditor->mainPos().x()) / 16;
        int blockY = (event->pos().y()-ui->glMapEditor->mainPos().y()) / 16;
        int indexN, indexW, indexT, indexS;

        Npc *eventN = nullptr;
        Warp *eventW = nullptr;
        Trigger *eventT = nullptr;
        Sign *eventS = nullptr;

        // Find the NPC entity at that position
        for (int i = 0; i < m_CurrentMap->entities().npcs().size(); i++)
        {
            Npc *npc = m_CurrentMap->entities().npcs()[i];
            if (npc->positionX == blockX && npc->positionY == blockY)
            {
                eventN = npc;
                indexN = i;
                break;
            }
        }
        if (eventN != NULL)
        {
            // Load NPC properties
            ui->stckEntityEditor->setCurrentWidget(ui->pageNPCs);
            ui->npc_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(eventN->offset, 16).toUpper());
            ui->npc_num->setValue(eventN->npcID);
            ui->npc_sprite->setValue(eventN->imageID);
            ui->npc_pos_x->setValue(eventN->positionX);
            ui->npc_pos_y->setValue(eventN->positionY);
            ui->spnNPCHeight->setValue(eventN->level);
            ui->npc_replacement->setValue(eventN->replacement);
            ui->spnNPCIdleAnim->setValue(eventN->behaviour);
            ui->npc_mov_x->setValue(eventN->moveRadiusX);
            ui->npc_mov_y->setValue(eventN->moveRadiusY);
            ui->npc_view_rad->setValue(eventN->viewRadius);
            ui->npc_trainer->setValue(eventN->property);
            ui->npc_script->setValue(eventN->ptrScript);
            ui->npc_flag->setValue(eventN->flag);
            ui->npc_raw_data->setData(eventN->rawData());
            ui->spnEntityScroller->setValue(indexN);

            CurrentEntity entity;
            entity.absPos.setX((event->pos().x()/16)*16);
            entity.absPos.setY((event->pos().y()/16)*16);
            entity.type = ET_Npc;
            entity.entity = eventN;
            entity.index = indexN;
            ui->glEntityEditor->setCurrentEntity(entity);

            return;
        }

        // Find the warp entity at that position
        for (int i = 0; i < m_CurrentMap->entities().warps().size(); i++)
        {
            Warp *warp = m_CurrentMap->entities().warps()[i];
            if (warp->positionX == blockX && warp->positionY == blockY)
            {
                eventW = warp;
                indexW = i;
                break;
            }
        }
        if (eventW != NULL)
        {
            // Load warp properties
            ui->stckEntityEditor->setCurrentWidget(ui->pageWarps);
            ui->warp_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(eventW->offset, 16).toUpper());
            ui->warp_pos_x->setValue(eventW->positionX);
            ui->warp_pos_y->setValue(eventW->positionY);
            ui->warp_number->setValue(eventW->warp);
            ui->warp_map->setValue(eventW->map);
            ui->warp_bank->setValue(eventW->bank);
            ui->spnWarpHeight->setValue(eventW->level);
            ui->warp_raw_data->setData(eventW->rawData());
            ui->spnEntityScroller->setValue(indexW);

            checkWarp();

            CurrentEntity entity;
            entity.absPos.setX((event->pos().x()/16)*16);
            entity.absPos.setY((event->pos().y()/16)*16);
            entity.type = ET_Warp;
            entity.entity = eventW;
            entity.index = indexW;
            ui->glEntityEditor->setCurrentEntity(entity);

            return;
        }

        // Find the trigger entity at that position
        for (int i = 0; i < m_CurrentMap->entities().triggers().size(); i++)
        {
            Trigger *trigger = m_CurrentMap->entities().triggers()[i];
            if (trigger->positionX == blockX && trigger->positionY == blockY)
            {
                eventT = trigger;
                indexT = i;
                break;
            }
        }
        if (eventT != NULL)
        {
            // Load trigger properties
            ui->stckEntityEditor->setCurrentWidget(ui->pageTriggers);
            ui->trigger_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(eventT->offset, 16).toUpper());
            ui->trigger_pos_x->setValue(eventT->positionX);
            ui->trigger_pos_y->setValue(eventT->positionY);
            ui->trigger_var->setValue(eventT->variable);
            ui->trigger_value->setValue(eventT->value);
            ui->trigger_script->setValue(eventT->ptrScript);
            ui->spnTriggerHeight->setValue(eventT->level);
            ui->trigger_raw_data->setData(eventT->rawData());
            ui->spnEntityScroller->setValue(indexT);

            CurrentEntity entity;
            entity.absPos.setX((event->pos().x()/16)*16);
            entity.absPos.setY((event->pos().y()/16)*16);
            entity.type = ET_Trigger;
            entity.entity = eventT;
            entity.index = indexT;
            ui->glEntityEditor->setCurrentEntity(entity);

            return;
        }

        // Find the sign entity at that position
        for (int i = 0; i < m_CurrentMap->entities().signs().size(); i++)
        {
            Sign *sign = m_CurrentMap->entities().signs()[i];
            if (sign->positionX == blockX && sign->positionY == blockY)
            {
                eventS = sign;
                indexS = i;
                break;
            }
        }
        if (eventS != NULL)
        {
            // Load sign properties
            ui->stckEntityEditor->setCurrentWidget(ui->pageSigns);
            ui->sign_group_raw->setTitle(tr("Raw Data @ 0x") + QString::number(eventS->offset, 16).toUpper());
            ui->sign_pos_x->setValue(eventS->positionX);
            ui->sign_pos_y->setValue(eventS->positionY);
            ui->sign_script->setValue(eventS->ptrScript);
            ui->sign_raw_data->setData(eventS->rawData());
            ui->spnSignHeight->setValue(eventS->level);
            ui->spnSignType->setValue(static_cast<int>(eventS->type));
            ui->spnEntityScroller->setValue(indexS);

            CurrentEntity entity;
            entity.absPos.setX((event->pos().x()/16)*16);
            entity.absPos.setY((event->pos().y()/16)*16);
            entity.type = ET_Sign;
            entity.entity = eventS;
            entity.index = indexS;
            ui->glEntityEditor->setCurrentEntity(entity);

            showCorrectSignType(eventS);

            return;
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::entity_doubleClick(QMouseEvent *event)
    {
        // Map coordinates to block-coordinates
        int blockX = (event->pos().x()-ui->glMapEditor->mainPos().x()) / 16;
        int blockY = (event->pos().y()-ui->glMapEditor->mainPos().y()) / 16;
        int indexN, indexW, indexT, indexS;

        Npc *eventN = nullptr;
        Warp *eventW = nullptr;
        Trigger *eventT = nullptr;
        Sign *eventS = nullptr;

        // Find the NPC entity at that position
        for (int i = 0; i < m_CurrentMap->entities().npcs().size(); i++)
        {
            Npc *npc = m_CurrentMap->entities().npcs()[i];
            if (npc->positionX == blockX && npc->positionY == blockY)
            {
                eventN = npc;
                indexN = i;
                break;
            }
        }
        if (eventN != NULL)
        {
            if (!SETTINGS(ScriptEditor).isEmpty())
                openScript(ui->npc_script->value());
            return;
        }

        // Find the warp entity at that position
        for (int i = 0; i < m_CurrentMap->entities().warps().size(); i++)
        {
            Warp *warp = m_CurrentMap->entities().warps()[i];
            if (warp->positionX == blockX && warp->positionY == blockY)
            {
                eventW = warp;
                indexW = i;
                break;
            }
        }
        if (eventW != NULL)
        {
            m_CurrentWarp = ui->warp_number->value();
            loadMapChangeTreeView(ui->warp_bank->value(), ui->warp_map->value());
            return;
        }

        // Find the trigger entity at that position
        for (int i = 0; i < m_CurrentMap->entities().triggers().size(); i++)
        {
            Trigger *trigger = m_CurrentMap->entities().triggers()[i];
            if (trigger->positionX == blockX && trigger->positionY == blockY)
            {
                eventT = trigger;
                indexT = i;
                break;
            }
        }
        if (eventT != NULL)
        {
            if (!SETTINGS(ScriptEditor).isEmpty())
                openScript(ui->trigger_script->value());
            return;
        }

        // Find the sign entity at that position
        for (int i = 0; i < m_CurrentMap->entities().signs().size(); i++)
        {
            Sign *sign = m_CurrentMap->entities().signs()[i];
            if (sign->positionX == blockX && sign->positionY == blockY)
            {
                eventS = sign;
                indexS = i;
                break;
            }
        }
        if (eventS != NULL)
        {
            if (ui->sign_base_id->value() <= ST_ScriptLeft && !SETTINGS(ScriptEditor).isEmpty())
            {
                openScript(ui->sign_script->value());
            }
            return;
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   12/11/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_tabWidget_currentChanged(int index)
    {
        QPoint scrollPos = ui->glMapEditor->mainPos();

        QSize size;
        if(ui->glMapEditor->mainMap() != NULL)
            size = ui->glMapEditor->mainMap()->header().size();
        else
            size = ui->glMapEditor->layoutHeader()->size();

        if (index == 0)
        {
            ui->scrollArea->horizontalScrollBar()->setValue(scrollPos.x() - (ui->scrollArea->viewport()->width() -
                                                             size.width() * 16) / 2);
            ui->scrollArea->verticalScrollBar()->setValue(scrollPos.y() - (ui->scrollArea->viewport()->height() -
                                                           size.height() * 16) / 2);
        }
        else if (index == 1)
        {
            ui->scrollArea_5->horizontalScrollBar()->setValue(scrollPos.x() - (ui->scrollArea_5->viewport()->width() -
                                                               size.width() * 16) / 2);
            ui->scrollArea_5->verticalScrollBar()->setValue(scrollPos.y() - (ui->scrollArea_5->viewport()->height() -
                                                             size.height() * 16) / 2);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/27/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_warp_number_valueChanged(int value)
    {
        checkWarp();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/27/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_warp_map_valueChanged(int value)
    {
        checkWarp();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/27/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_warp_bank_valueChanged(int value)
    {
        checkWarp();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/29/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::checkWarp()
    {
    int tempBank = ui->warp_bank->value();
    int tempMap = ui->warp_map->value();
    int tempNum = ui->warp_number->value();
    if (tempBank < dat_MapBankTable->banks().size() &&
        tempMap < dat_MapBankTable->banks().at(tempBank)->maps().size() &&
        tempNum < dat_MapBankTable->banks()[tempBank]->maps()[tempMap]->entities().warps().size())
        ui->btnWarpToDest->setEnabled(true);
    else
        ui->btnWarpToDest->setEnabled(false);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/2/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::showCorrectSignType(Sign *sign)
    {
        if (sign->type <= ST_ScriptLeft)
        {
            ui->ctrSignScript->setVisible(true);
            ui->ctrSignItem->setVisible(false);
            ui->ctrSignBase->setVisible(false);
            ui->sign_script->setValue(sign->ptrScript);
        }
        else if (sign->type == ST_SecretBase)
        {
            ui->ctrSignScript->setVisible(false);
            ui->ctrSignItem->setVisible(false);
            ui->ctrSignBase->setVisible(true);
            ui->sign_base_id->setValue(sign->baseID);
        }
        else
        {
            ui->ctrSignScript->setVisible(false);
            ui->ctrSignItem->setVisible(true);
            ui->ctrSignBase->setVisible(false);
            ui->spnSignItem->setValue(sign->item);
            ui->sign_item_hidden->setValue(sign->hiddenID);
            ui->sign_item_amount->setValue(sign->amount);
            ui->chkSignItemExact->setChecked(sign->exactRequired);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::closeEvent(QCloseEvent *event)
    {
        // Destroys OpenGL objects
        delete ui->glBorderEditor;
        delete ui->glMapEditor;
        delete ui->glBlockEditor;
        delete ui->glEntityEditor;

        ui->glBorderEditor = NULL;
        ui->glMapEditor = NULL;
        ui->glBlockEditor = NULL;
        ui->glEntityEditor = NULL;

        // Destroys the window
        QMainWindow::closeEvent(event);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/4/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_lineEdit_textChanged(const QString &arg1)
    {
        m_proxyModel.setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive,
                                                                   QRegExp::FixedString));
        const QModelIndex index = m_proxyModel.mapFromSource(*m_CurrentMap->getTreeViewIndex());
        if (index.isValid())
            changeTreeViewMap(index);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::RecentFile_triggered()
    {
        if(loadROM(((QAction*)sender())->data().toString()))
            loadMapData();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionClearRecentFiles_triggered()
    {
        CHANGESETTING(RecentFiles, QList<QString>());
        Settings::write();
        ui->actionRecent_Files->setMenu(new QMenu);
        ui->actionRecent_Files->setEnabled(false);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/6/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_tabWidget_2_currentChanged(int index)
    {
        if (index == -1)
            return;

        if (index == 0)
        {
            ui->glMapEditor->setMovementMode(false);
            ui->glMapEditor->update();
        }
        else
        {
            ui->glMapEditor->setMovementMode(true);
            ui->glMapEditor->update();
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/6/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_action_Connection_Editor_triggered()
    {

    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/7/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_action_Settings_triggered()
    {
        SettingsDialog settingsDialog(this);
        settingsDialog.exec();
        setScriptEditorButtonsEnabled(!SETTINGS(ScriptEditor).isEmpty());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome, Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   9/23/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_action_Tileset_Editor_triggered()
    {
        TilesetDialog tilesetDialog(this, ui->glMapEditor, ui->glBlockEditor);
        tilesetDialog.exec();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome, Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/26/2017
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::toggle_grid(bool checked)
    {
        ui->glMapEditor->setGridVisible(checked);
        ui->glBlockEditor->setGridVisible(checked);
        ui->glEntityEditor->setGridVisible(checked);
        CHANGESETTING(ShowGrid, checked);
        Settings::write();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_btnWarpToDest_clicked()
    {
        m_CurrentWarp = ui->warp_number->value();
        loadMapChangeTreeView(ui->warp_bank->value(), ui->warp_map->value());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::openScript(UInt32 scriptAddr)
    {
        if (SETTINGS(ScriptEditor).isEmpty())
        {
            Messages::showMessage(this, tr("You have not yet selected a script editor.\nPlease choose a script editor from the Settings dialog."));
            return false;
        }
        QStringList args;
        args << QDir::toNativeSeparators(m_Rom.info().path()) << QString::number(scriptAddr, 16);
        return QProcess::startDetached(SETTINGS(ScriptEditor), args);
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/8/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_btnNPCOpenScript_clicked()
    {
        openScript(ui->npc_script->value());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/8/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_btnTriggerOpenScript_clicked()
    {
        openScript(ui->trigger_script->value());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/8/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_btnSignOpenScript_clicked()
    {
        openScript(ui->sign_script->value());
    }
}
