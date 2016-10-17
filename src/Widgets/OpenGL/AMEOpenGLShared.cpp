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
#include <AME/System/Configuration.hpp>
#include <AME/Widgets/OpenGL/AMEOpenGLShared.hpp>
#include <QBoy/OpenGL/GLErrors.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Definitions
    //
    ///////////////////////////////////////////////////////////
    QOpenGLShaderProgram m_Program;
    QOpenGLShaderProgram m_PmtProgram;
    QOpenGLShaderProgram m_RgbProgram;
    QOpenGLVertexArrayObject m_VAO;


    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   10/17/2016
    //
    ///////////////////////////////////////////////////////////
    AMEOpenGLShared::~AMEOpenGLShared()
    {
        if (m_VAO.isCreated())
        {
            m_VAO.destroy();
            m_Program.removeAllShaders();
            m_PmtProgram.removeAllShaders();
            m_RgbProgram.removeAllShaders();
            m_Program.release();
            m_PmtProgram.release();
            m_RgbProgram.release();
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   10/17/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEOpenGLShared::initializeGL()
    {
        if (qboy::GLErrors::Current == NULL)
            qboy::GLErrors::Current = new qboy::GLErrors;


        // Initializes the OpenGL functions
        initializeOpenGLFunctions();


        // Initializes the 8-bit shader program
        m_Program.create();
        m_Program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/MapVertexShader.glsl");
        m_Program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/MapFragmentShader.glsl");
        m_Program.link();
        m_Program.bind();
        m_Program.setUniformValue("smp_texture", 0);
        m_Program.setUniformValue("smp_palette", 1);

        // Initializes the primitive shader program
        m_PmtProgram.create();
        m_PmtProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/PrimitiveVertexShader.glsl");
        m_PmtProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/PrimitiveFragmentShader.glsl");
        m_PmtProgram.link();

        // Initializes the RGBA shader program
        m_RgbProgram.create();
        m_RgbProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/NormalVertexShader.glsl");
        m_RgbProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/NormalFragmentShader.glsl");
        m_RgbProgram.link();
        m_RgbProgram.bind();
        m_RgbProgram.setUniformValue("smp_texture", 0);

        // Initializes the VAO
        m_VAO.create();
        m_VAO.bind();
    }
}
