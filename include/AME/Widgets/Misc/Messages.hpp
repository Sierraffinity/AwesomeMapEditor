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


#ifndef __AME_MESSAGES_HPP__
#define __AME_MESSAGES_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QtWidgets>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    Configuration.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/16/2016
    /// \brief   Displays messages of various kinds.
    ///
    ///////////////////////////////////////////////////////////
    class Messages {
    public:

        ///////////////////////////////////////////////////////////
        /// \param Shows a vanilla information box.
        ///
        /// Displays the specified text in a message box that has
        /// an information icon on the top-left.
        ///
        /// \param parent The parental window
        /// \param text Text to display in the msgbox
        ///
        ///////////////////////////////////////////////////////////
        static void showMessage(QWidget *parent, const QString &text);

        ///////////////////////////////////////////////////////////
        /// \brief Shows a question box and returns the result.
        ///
        /// Displays the specified question in a message box and
        /// shows a Yes/No button.
        ///
        /// \param parent The parental window
        /// \param text Question to display in the msgbox
        /// \returns true if the "yes" button was clicked.
        ///
        ///////////////////////////////////////////////////////////
        static bool showQuestion(QWidget *parent, const QString &text);

        ///////////////////////////////////////////////////////////
        /// \brief Shows an error box.
        ///
        /// Displays the specified error message in a message box.
        ///
        /// \param parent The parental window
        /// \param text Error to display in the msgbox
        ///
        ///////////////////////////////////////////////////////////
        static void showError(QWidget *parent, const QString &text);
    };
}


#endif //__AME_MESSAGES_HPP__
