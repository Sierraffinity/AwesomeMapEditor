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


#ifndef __AME_AMEIMAGEBUTTON_H__
#define __AME_AMEIMAGEBUTTON_H__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QtWidgets>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    AMEImageButton.h
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/18/2016
    /// \brief   Defines a button that shows an image only.
    ///
    ///////////////////////////////////////////////////////////
    class AMEImageButton : public QPushButton {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes AMEImageButton with a given parent.
        ///
        ///////////////////////////////////////////////////////////
        AMEImageButton(QWidget *parent = NULL);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Destroys the OpenGL objects.
        ///
        ///////////////////////////////////////////////////////////
        ~AMEImageButton();


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Defaults the first button to be the normal one.
        ///
        ///////////////////////////////////////////////////////////
        void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Changes the button appearance on hovering.
        ///
        ///////////////////////////////////////////////////////////
        void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Changes the button appearance on leaving.
        ///
        ///////////////////////////////////////////////////////////
        void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Changes the button appearance on pressing.
        ///
        ///////////////////////////////////////////////////////////
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Changes the button appearance on releasing.
        ///
        ///////////////////////////////////////////////////////////
        void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Paints the currently active image.
        ///
        ///////////////////////////////////////////////////////////
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QPixmap m_CurrentImage;
    };
}


#endif //__AME_AMEIMAGEBUTTON_H__
