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
#include <AME/Widgets/OpenGL/AMEMapView.h>
#include <AME/Forms/MainWindow.h>
#include <AME/Forms/ErrorWindow.h>
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QtEvents>


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
        m_lastOpenedMap = NULL;

        connect(ui->openGLWidget_5, SIGNAL(onMouseClick(QMouseEvent*)), this, SLOT(on_entity_mouseClick(QMouseEvent*)));
        if (!Settings::parse())
            return;         // TODO: create default config file if none exists
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
                clearBeforeLoading();
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
    void MainWindow::clearBeforeLoading()
    {
        // First clears the tree-view
        ui->treeView->collapseAll();
        ui->treeView->clear();

        // Clears all the OpenGL widgets
        ui->openGLWidget->freeGL();
        ui->openGLWidget_2->freeGL();
        ui->openGLWidget_3->freeGL();
        ui->openGLWidget_5->freeGL();

        // Sets the tab index to the map-index
        ui->tabWidget->setCurrentIndex(0);
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
            ErrorWindow errorWindow(this);
            errorWindow.exec();
            return;
        }

        setupAfterLoading();
        m_Rom.clearCache();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setupAfterLoading()
    {
        QStandardItemModel *model = new QStandardItemModel;
        for (unsigned i = 0; i < CONFIG(PokemonCount); i++)
        {
            QStandardItem *item = new QStandardItem;
            item->setText(dat_PokemonTable->names().at(i));
            item->setIcon(QIcon(QPixmap::fromImage(dat_PokemonTable->images().at(i))));
            model->appendRow(item);
        }

        // Fills all wild-pokemon comboboxes with the names
        foreach (QComboBox *box, ui->tabWidget_3->findChildren<QComboBox *>())
            box->setModel(model);

        // Sets the max pokemon IDs within the spinboxes
        foreach (QSpinBox *box, ui->tabWidget_3->findChildren<QSpinBox *>(QRegularExpression("sbWild")))
            box->setRange(0, CONFIG(PokemonCount));


        // Fills the tree-view with all the maps
        //QTreeWidgetItem *topItem = new QTreeWidgetItem;
        //topItem->setText(0, "MapBanks");
        ui->treeView->setUpdatesEnabled(false);
        //ui->treeView->addTopLevelItem(topItem);

        // Creates icons for use in treeView
        QIcon mapFolderIcon;
        mapFolderIcon.addFile(QStringLiteral(":/icons/folder_closed_map.ico"), QSize(), QIcon::Normal, QIcon::Off);
        mapFolderIcon.addFile(QStringLiteral(":/icons/folder_map.ico"), QSize(), QIcon::Normal, QIcon::On);

        QIcon folderIcon;
        folderIcon.addFile(QStringLiteral(":/icons/folder_closed.ico"), QSize(), QIcon::Normal, QIcon::Off);

        QIcon mapIcon;
        mapIcon.addFile(QStringLiteral(":/icons/map.ico"), QSize(), QIcon::Normal, QIcon::Off);
        mapIcon.addFile(QStringLiteral(":/icons/image.ico"), QSize(), QIcon::Normal, QIcon::On);

        switch(SETTINGS(MapSortOrder))
        {
            case MSO_Name:
            {
                // Adds every map name to the tree. Format: [<index>] <map name>
                int nameCount = CONFIG(MapNameCount);
                int nameTotal = CONFIG(MapNameTotal);
                for (int i = 0; i < nameCount; i++)
                {
                    QTreeWidgetItem *nameItem = new QTreeWidgetItem;
                    nameItem->setIcon(0, folderIcon);

                    // Specifies the display name of the bank and adds it to the treeview
                    nameItem->setText(0, '[' +
                                      QString("%1").arg(i + nameTotal - nameCount, 2 , 16, QChar('0')).toUpper() +
                                      "] " +
                                      dat_MapNameTable->names()[i]->name);
                    ui->treeView->addTopLevelItem(nameItem);
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
                        QTreeWidgetItem *nameItem = ui->treeView->topLevelItem(nameIndex);
                        nameItem->setIcon(0, mapFolderIcon);

                        QTreeWidgetItem *mapItem = new QTreeWidgetItem(nameItem);
                        mapItem->setIcon(0, mapIcon);

                        mapItem->setText(0,
                            '[' +
                            QString("%1").arg(i, 2 , 16, QChar('0')).toUpper() +
                            ", " +
                            QString("%1").arg(j, 2 , 16, QChar('0')).toUpper() +
                            "] " +
                            dat_MapNameTable->names()[nameIndex]->name);

                        // Sets properties to identify map on click
                        QByteArray array;
                        array.append(i);
                        array.append(j);
                        mapItem->setData(0, Qt::UserRole, array);

                        // Adds the map to the bank
                        //bankItem->addChild(mapItem);
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
                    //QTreeWidgetItem *bankItem = new QTreeWidgetItem(topItem);
                    QTreeWidgetItem *bankItem = new QTreeWidgetItem;
                    MapBank *bank = dat_MapBankTable->banks()[i];
                    bankItem->setIcon(0, mapFolderIcon);

                    // Adds all the bank's maps
                    int mapCount = bank->maps().size();
                    for (int j = 0; j < mapCount; j++)
                    {
                        QTreeWidgetItem *mapItem = new QTreeWidgetItem(bankItem);
                        Map *map = bank->maps()[j];
                        mapItem->setIcon(0, mapIcon);

                        // Specifies the display name
                        /*mapItem->setText(0,
                            QString("(")        +
                            QString::number(i)  +
                            QString(".")        +
                            QString::number(j)  +
                            QString(") ") + map->name()
                        );*/
                        mapItem->setText(0,
                            "[" +
                            QString("%1").arg(i, 2 , 16, QChar('0')).toUpper() +
                            ", " +
                            QString("%1").arg(j, 2 , 16, QChar('0')).toUpper() +
                            "] " +
                            dat_MapNameTable->names()[map->nameIndex() + CONFIG(MapNameCount) - CONFIG(MapNameTotal)]->name);

                        // Sets properties to identify map on click
                        QByteArray array;
                        array.append(i);
                        array.append(j);
                        mapItem->setData(0, Qt::UserRole, array);

                        // Adds the map to the bank
                        bankItem->addChild(mapItem);
                    }

                    // Specifies the display name of the bank and adds it to the treeview
                    //bankItem->setText(0, QString("Bank #") + QString::number(i));
                    bankItem->setText(0, '[' + QString("%1").arg(i, 2 , 16, QChar('0')).toUpper() + ']');
                    ui->treeView->addTopLevelItem(bankItem);
                }
                break;
            }
            default:
                break;
        }

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
    // Last edit by:   Diegoisawesome
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_treeView_itemDoubleClicked(QTreeWidgetItem *item, int column)
    {
        //if (item->parent() == NULL || item->parent()->parent() == NULL)
        if (item->parent() == NULL)
            return;

        // Switch icon
        if(m_lastOpenedMap != NULL)
            m_lastOpenedMap->setExpanded(false);
        item->setExpanded(true);
        m_lastOpenedMap = item;


        // Clears all the OpenGL widgets
        ui->openGLWidget->freeGL();
        ui->openGLWidget_2->freeGL();
        ui->openGLWidget_3->freeGL();
        ui->openGLWidget_5->freeGL();


        // Retrieves the new map from the stored property
        QByteArray data = item->data(column, Qt::UserRole).toByteArray();
        Map *currentMap = dat_MapBankTable->banks()[data.at(0)]->maps()[data.at(1)];

        // Fills all the OpenGL widgets
        ui->openGLWidget_2->setMap(m_Rom, currentMap);
        ui->openGLWidget_2->makeGL();
        ui->openGLWidget_2->update();
        ui->openGLWidget_3->setMapView(ui->openGLWidget_2);
        ui->openGLWidget_3->update();
        ui->openGLWidget->setMapView(ui->openGLWidget_2);
        ui->openGLWidget->update();
        ui->openGLWidget_5->setEntities(currentMap);
        ui->openGLWidget_5->setMapView(ui->openGLWidget_2);
        ui->openGLWidget_5->update();

        if (ui->tabWidget->currentIndex() == 1)
        {
            ui->openGLWidget_2->show();
        }
        m_CurrentMap = currentMap;

        // Fills the wild-pokemon tab
        setupWildPokemon(currentMap);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_entity_mouseClick(QMouseEvent *event)
    {
        // Map coordinates to block-coordinates
        int blockX = (event->pos().x()-ui->openGLWidget_2->mainPos().x()) / 16;
        int blockY = (event->pos().y()-ui->openGLWidget_2->mainPos().y()) / 16;
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
                break;
            }
        }
        if (eventN != NULL)
        {
            // Load NPC properties
            ui->stckEntityEditor->setCurrentWidget(ui->pageNPCs);
            ui->npc_group_raw->setTitle(QString("Raw Data @ 0x") + QString::number(eventN->offset, 16));
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
            ui->npc_raw_data->setData(eventN->rawData);

            CurrentEntity entity;
            entity.absPos.setX((event->pos().x()/16)*16);
            entity.absPos.setY((event->pos().y()/16)*16);
            entity.type = ET_Npc;
            entity.entity = eventN;
            ui->openGLWidget_5->setCurrentEntity(entity);
            ui->openGLWidget_5->update();

            return;
        }

        // Find the warp entity at that position
        for (int i = 0; i < m_CurrentMap->entities().warps().size(); i++)
        {
            Warp *warp = m_CurrentMap->entities().warps()[i];
            if (warp->positionX == blockX && warp->positionY == blockY)
            {
                eventW = warp;
                break;
            }
        }
        if (eventW != NULL)
        {
            // Load warp properties
            ui->stckEntityEditor->setCurrentWidget(ui->pageWarps);
            ui->warp_group_raw->setTitle(QString("Raw Data @ 0x") + QString::number(eventW->offset, 16));
            ui->warp_pos_x->setValue(eventW->positionX);
            ui->warp_pos_y->setValue(eventW->positionY);
            ui->warp_number->setValue(eventW->warp);
            ui->warp_map->setValue(eventW->map);
            ui->warp_bank->setValue(eventW->bank);
            ui->spnWarpHeight->setValue(eventW->level);
            ui->warp_raw_data->setData(eventW->rawData);

            CurrentEntity entity;
            entity.absPos.setX((event->pos().x()/16)*16);
            entity.absPos.setY((event->pos().y()/16)*16);
            entity.type = ET_Warp;
            entity.entity = eventW;
            ui->openGLWidget_5->setCurrentEntity(entity);
            ui->openGLWidget_5->update();

            return;
        }

        // Find the trigger entity at that position
        for (int i = 0; i < m_CurrentMap->entities().triggers().size(); i++)
        {
            Trigger *trigger = m_CurrentMap->entities().triggers()[i];
            if (trigger->positionX == blockX && trigger->positionY == blockY)
            {
                eventT = trigger;
                break;
            }
        }
        if (eventT != NULL)
        {
            // Load warp properties
            ui->stckEntityEditor->setCurrentWidget(ui->pageTriggers);
            ui->trigger_group_raw->setTitle(QString("Raw Data @ 0x") + QString::number(eventT->offset, 16));
            ui->trigger_pos_x->setValue(eventT->positionX);
            ui->trigger_pos_y->setValue(eventT->positionY);
            ui->trigger_var->setValue(eventT->variable);
            ui->trigger_value->setValue(eventT->value);
            ui->trigger_script->setValue(eventT->ptrScript);
            ui->spnTriggerHeight->setValue(eventT->level);
            ui->trigger_raw_data->setData(eventT->rawData);

            CurrentEntity entity;
            entity.absPos.setX((event->pos().x()/16)*16);
            entity.absPos.setY((event->pos().y()/16)*16);
            entity.type = ET_Trigger;
            entity.entity = eventT;
            ui->openGLWidget_5->setCurrentEntity(entity);
            ui->openGLWidget_5->update();

            return;
        }

        // Find the sign entity at that position
        for (int i = 0; i < m_CurrentMap->entities().signs().size(); i++)
        {
            Sign *sign = m_CurrentMap->entities().signs()[i];
            if (sign->positionX == blockX && sign->positionY == blockY)
            {
                eventS = sign;
                break;
            }
        }
        if (eventS != NULL)
        {
            // Load warp properties
            ui->stckEntityEditor->setCurrentWidget(ui->pageSigns);
            ui->sign_group_raw->setTitle(QString("Raw Data @ 0x") + QString::number(eventS->offset, 16));
            ui->sign_pos_x->setValue(eventS->positionX);
            ui->sign_pos_y->setValue(eventS->positionY);
            ui->sign_script->setValue(eventS->ptrScript);
            ui->sign_raw_data->setData(eventS->rawData);
            ui->spnSignHeight->setValue(eventS->level);
            ui->spnSignType->setValue(static_cast<int>(eventS->type));

            CurrentEntity entity;
            entity.absPos.setX((event->pos().x()/16)*16);
            entity.absPos.setY((event->pos().y()/16)*16);
            entity.type = ET_Sign;
            entity.entity = eventS;
            ui->openGLWidget_5->setCurrentEntity(entity);
            ui->openGLWidget_5->update();

            if (eventS->type <= ST_ScriptLeft)
            {
                ui->sign_script->setValue(eventS->ptrScript);
            }

            return;
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
        delete ui->openGLWidget;
        delete ui->openGLWidget_2;
        delete ui->openGLWidget_3;
        delete ui->openGLWidget_4;
        delete ui->openGLWidget_5;

        ui->openGLWidget = NULL;
        ui->openGLWidget_2 = NULL;
        ui->openGLWidget_3 = NULL;
        ui->openGLWidget_4 = NULL;
        ui->openGLWidget_5 = NULL;

        // Destroys the window
        QMainWindow::closeEvent(event);
    }
}
