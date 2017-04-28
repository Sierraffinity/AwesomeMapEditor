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


#include <AME/System/ErrorStack.hpp>
#include <AME/Forms/ErrorWindow.h>
#include "ui_ErrorWindow.h"


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    ErrorWindow::ErrorWindow(QWidget *parent) :
        QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint),
        ui(new Ui::ErrorWindow),
        m_CurrentIndex(0)
    {
        ui->setupUi(this);
        setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
        setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);
        show();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    ErrorWindow::~ErrorWindow()
    {
        delete ui;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void ErrorWindow::showEvent(QShowEvent *event)
    {
        Q_UNUSED(event);
        QString method = ErrorStack::methods()[0];
        QString msg = ErrorStack::errors()[0];

        // Loads the first error
        int scopePos = method.indexOf("::");
        int arguments = method.indexOf("(");
        int possible = 0;
        if ((possible = method.indexOf("::", scopePos+1)) != -1)
            scopePos = possible;

        ui->class_label->setText(method.left(scopePos));
        ui->func_label->setText(method.mid(scopePos+2, arguments-(scopePos+2)));
        ui->desc_label->setText(msg.replace("\n", " "));

        // Updates the information label
        ui->info_label->setText(QString("Showing error %0 of %1.").arg(1).arg(ErrorStack::errors().size()));

        // Performs some bound checking for the buttons
        ui->pushButton->setEnabled(false);
        if (ErrorStack::errors().size() == 1)
            ui->pushButton_2->setEnabled(false);
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void ErrorWindow::on_pushButton_clicked()
    {
        ui->pushButton_2->setEnabled(true);
        m_CurrentIndex--;
        if (m_CurrentIndex == 0)
            ui->pushButton->setEnabled(false);


        // Update information
        QString method = ErrorStack::methods()[m_CurrentIndex];
        QString msg = ErrorStack::errors()[m_CurrentIndex];

        int scopePos = method.indexOf("::");
        int arguments = method.indexOf("(");
        int possible = 0;
        if ((possible = method.indexOf("::", scopePos+1)) != -1)
            scopePos = possible;

        ui->class_label->setText(method.left(scopePos));
        ui->func_label->setText(method.mid(scopePos+2, arguments-(scopePos+2)));
        ui->desc_label->setText(msg.replace("\n", " "));

        // Updates the information label
        ui->info_label->setText(QString("Showing error %0 of %1.").arg(m_CurrentIndex).arg(ErrorStack::errors().size()));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void ErrorWindow::on_pushButton_2_clicked()
    {
        ui->pushButton->setEnabled(true);
        m_CurrentIndex++;
        if (m_CurrentIndex == (uint)ErrorStack::errors().size()-1)
            ui->pushButton_2->setEnabled(false);


        // Update information
        QString method = ErrorStack::methods()[m_CurrentIndex];
        QString msg = ErrorStack::errors()[m_CurrentIndex];

        int scopePos = method.indexOf("::");
        int arguments = method.indexOf("(");
        int possible = 0;
        if ((possible = method.indexOf("::", scopePos+1)) != -1)
            scopePos = possible;

        ui->class_label->setText(method.left(scopePos));
        ui->func_label->setText(method.mid(scopePos+2, arguments-(scopePos+2)));
        ui->desc_label->setText(msg.replace("\n", " "));

        // Updates the information label
        ui->info_label->setText(QString("Showing error %0 of %1.").arg(m_CurrentIndex).arg(ErrorStack::errors().size()));
    }
}
