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


#ifndef __AME_AMEBORDERVIEW_HPP__
#define __AME_AMEBORDERVIEW_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QtWidgets>
#include <QtOpenGL/QtOpenGL>
#include <AME/Widgets/OpenGL/AMEMapView.h>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    AMEBorderView.h
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/19/2016
    /// \brief   Displays the entire border block.
    ///
    ///////////////////////////////////////////////////////////
    class AMEBorderView : public QOpenGLWidget, public QOpenGLFunctions {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes AMEBorderView with a given parent.
        ///
        ///////////////////////////////////////////////////////////
        AMEBorderView(QWidget *parent = NULL);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Destroys the OpenGL objects.
        ///
        ///////////////////////////////////////////////////////////
        ~AMEBorderView();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves pre-loaded data from the map view.
        ///
        ///////////////////////////////////////////////////////////
        void setMapView(AMEMapView *view, bool layout = false);

        ///////////////////////////////////////////////////////////
        /// \brief Destroys all OpenGL resources.
        ///
        ///////////////////////////////////////////////////////////
        void freeGL();


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Initializes OpenGL-related things.
        ///
        ///////////////////////////////////////////////////////////
        void initializeGL();

        ///////////////////////////////////////////////////////////
        /// \brief Resizes the viewport and redraws the objects.
        ///
        ///////////////////////////////////////////////////////////
        void resizeGL(int w, int h);

        ///////////////////////////////////////////////////////////
        /// \brief Renders all the map textures.
        ///
        ///////////////////////////////////////////////////////////
        void paintGL();


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt32 m_VertexBuffer;
        UInt32 m_BackTexture;
        UInt32 m_ForeTexture;
        UInt32 m_PalTexture;
        UInt32 m_IndexBuffer;
        QOpenGLVertexArrayObject m_VAO;
        QOpenGLShaderProgram m_Program;
        UInt8 *m_Foreground;
        UInt8 *m_Background;
    };
}


#endif //__AME_AMEBORDERVIEW_HPP__
