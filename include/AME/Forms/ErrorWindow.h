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


#ifndef __AME_ERRORWINDOW_H__
#define __AME_ERRORWINDOW_H__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QDialog>


namespace ame
{
    namespace Ui
    {
        class ErrorWindow;
    }


    ///////////////////////////////////////////////////////////
    /// \file    ErrorWindow.h
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/18/2016
    /// \brief   Holds components for the error window.
    ///
    ///////////////////////////////////////////////////////////
    class ErrorWindow : public QDialog {
    Q_OBJECT
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        /// \param parent Parental widget (optional)
        ///
        /// Creates all resources that are required by the GUI.
        ///
        ///////////////////////////////////////////////////////////
        explicit ErrorWindow(QWidget *parent = 0);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Releases all resources that belong to ErrorWindow.
        ///
        ///////////////////////////////////////////////////////////
        ~ErrorWindow();


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Bugfix: Close button invisible.
        ///
        ///////////////////////////////////////////////////////////
        void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;


    private slots:

        ///////////////////////////////////////////////////////////
        // Private slots
        //
        ///////////////////////////////////////////////////////////
        void on_pushButton_clicked();
        void on_pushButton_2_clicked();


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        Ui::ErrorWindow *ui;    ///< Gives access to the GUI objects
        uint m_CurrentIndex;    ///< Current error browsing index
    };

}


#endif // __AME_ERRORWINDOW_H__
