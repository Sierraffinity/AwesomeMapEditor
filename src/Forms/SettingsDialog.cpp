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
#include <AME/Forms/SettingsDialog.h>
#include "ui_SettingsDialog.h"
#include <AME/System/Settings.hpp>
#include <QDir>
#include <QFileDialog>


namespace ame {
    SettingsDialog::SettingsDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SettingsDialog)
    {
        ui->setupUi(this);
        if (!SETTINGS(ScriptEditor).isEmpty())
        {
            tempScriptEditor = SETTINGS(ScriptEditor);
            ui->lbScriptEditor->setText(QDir::toNativeSeparators(tempScriptEditor));
        }

        QString tempPrefix = SETTINGS(HexPrefix);
        if(tempPrefix == "0x")
            ui->radioButton->setChecked(true);
        else if(tempPrefix == "$")
            ui->radioButton_2->setChecked(true);
        else if(tempPrefix == "&H")
            ui->radioButton_3->setChecked(true);
        else
        {
            ui->radioButton_4->setChecked(true);
            ui->lineEdit->setText(tempPrefix);
        }

        ui->sldTranslucency->setValue(SETTINGS(Translucency));

        connect(this, SIGNAL(accepted()), this, SLOT(accepted()));
    }

    SettingsDialog::~SettingsDialog()
    {
        delete ui;
    }

    void SettingsDialog::accepted()
    {
        CHANGESETTING(ScriptEditor, tempScriptEditor);
        CHANGESETTING(Translucency, ui->sldTranslucency->value());
        //CHANGESETTING(Language, ui->comboBox->);

        QString tempPrefix;
        if(ui->radioButton->isChecked())
            tempPrefix = "0x";
        else if(ui->radioButton_2->isChecked())
            tempPrefix = "$";
        else if(ui->radioButton_3->isChecked())
            tempPrefix = "&H";
        else
            tempPrefix = ui->lineEdit->text();

        CHANGESETTING(HexPrefix, tempPrefix);
        Settings::write();
    }

    void SettingsDialog::on_sldTranslucency_valueChanged(int value)
    {
        ui->lbTranslucency->setText(QString::number(value) + QString("%"));
    }

    void SettingsDialog::on_btnScriptEditor_clicked()
    {
        QFileDialog *dialog = new QFileDialog(this, tr("Choose Script Editor"));
        dialog->setFilter(QDir::Executable | QDir::AllDirs);
#ifdef Q_OS_WIN
        dialog->setNameFilter(tr("Executable files (*.exe)"));
#endif
        dialog->setFileMode(QFileDialog::ExistingFile);
        if(dialog->exec())
        {
            QString file = dialog->selectedFiles()[0];

            // Determines whether the dialog was successful
            if (!file.isNull() && !file.isEmpty())
            {
                tempScriptEditor = file;
                ui->lbScriptEditor->setText(QDir::toNativeSeparators(tempScriptEditor));
            }
        }
        // Dialog cancelled
        return;
    }

}
