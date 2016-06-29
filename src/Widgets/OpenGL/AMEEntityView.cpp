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
#include <AME/Widgets/OpenGL/AMEEntityView.h>
#include <QBoy/OpenGL/GLErrors.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Local definitions
    //
    ///////////////////////////////////////////////////////////
    #define MV_VERTEX_ATTR      0
    #define MV_COORD_ATTR       1


    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    AMEEntityView::AMEEntityView(QWidget *parent)
        : QOpenGLWidget(parent),
          QOpenGLFunctions(),
          m_MapView(0)
    {
        QSurfaceFormat format = this->format();
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        //format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CompatibilityProfile);
        setFormat(format);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    AMEEntityView::~AMEEntityView()
    {
        if (m_VAO.isCreated())
        {
            freeGL();
            makeCurrent();

            /* Static data; only destroy on app close */
            glCheck(glDeleteBuffers(1, &m_VertexBuffer));
            glCheck(glDeleteBuffers(1, &m_IndexBuffer));
            glCheck(glDeleteTextures(1, &m_TextureAtlas));

            m_Program.removeAllShaders();
            m_VAO.destroy();
            doneCurrent();
        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::initializeGL()
    {
        initializeOpenGLFunctions();

        // Defines constant buffers
        const unsigned int ibuffer[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        // Initializes the VAO
        m_VAO.create();
        m_VAO.bind();

        // Initializes the shader program
        m_Program.create();
        m_Program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/NormalVertexShader.glsl");
        m_Program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/NormalFragmentShader.glsl");
        m_Program.link();

        // Initializes the buffers
        glCheck(glGenBuffers(1, &m_VertexBuffer));
        glCheck(glGenBuffers(1, &m_IndexBuffer));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
        glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, NULL, GL_DYNAMIC_DRAW));
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
        glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*6, ibuffer, GL_STATIC_DRAW));

        // Loads the image and creates a texture from it
        QImage image(":/images/Entities_16x16.png");
        glCheck(glGenTextures(1, &m_TextureAtlas));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_TextureAtlas));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits()));

        // Enables blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::resizeGL(int w, int h)
    {
        glCheck(glViewport(0, 0, w, h));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::paintGL()
    {
        if (!m_MapView)
            return;

        // Paints the map-view
        QPainter painter(this);
        painter.beginNativePainting();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //m_MapView->m_ShowSprites = true;
        m_MapView->paintGL();
        //m_MapView->m_ShowSprites = false;
        painter.endNativePainting();

        // Binds the buffers
        glCheck(m_VAO.bind());
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));

        // Binds the texture
        glCheck(glActiveTexture(GL_TEXTURE0));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_TextureAtlas));

        // Binds the program and associates some properties
        m_Program.bind();
        m_Program.enableAttributeArray(MV_VERTEX_ATTR);
        m_Program.enableAttributeArray(MV_COORD_ATTR);
        m_Program.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 4*sizeof(float));
        m_Program.setAttributeBuffer(MV_COORD_ATTR,  GL_FLOAT, 2*sizeof(float), 2, 4*sizeof(float));


        // Sets up the variable matrix
        QMatrix4x4 mat_mvp;
        mat_mvp.setToIdentity();
        mat_mvp.ortho(0, width(), height(), 0, -1, 1);

        // Iterates through the entities and draws them
        const float bNpc[16] = { 0, 0, 0.0f, 0.0f, 16, 0, 0.25f, 0.0f, 16, 16, 0.25f, 1.0f, 0, 16, 0.0f, 1.0f };
        glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, bNpc, GL_DYNAMIC_DRAW));

        foreach (const Npc *npc, m_Entities->npcs())
        {
            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(npc->positionX*16+m_Translation.x(), npc->positionY*16+m_Translation.y());
            m_Program.setUniformValue("uni_mvp", mat_mvp);

            glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
        }

        const float bWarp[16] = { 0, 0, 0.25f, 0.0f, 16, 0, 0.5f, 0.0f, 16, 16, 0.5f, 1.0f, 0, 16, 0.25f, 1.0f };
        glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, bWarp, GL_DYNAMIC_DRAW));

        foreach (const Warp *warp, m_Entities->warps())
        {
            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(warp->positionX*16+m_Translation.x(), warp->positionY*16+m_Translation.y());
            m_Program.setUniformValue("uni_mvp", mat_mvp);

            glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
        }

        const float bTrigg[16] = { 0, 0, 0.5f, 0.0f, 16, 0, 0.75f, 0.0f, 16, 16, 0.75f, 1.0f, 0, 16, 0.5f, 1.0f };
        glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, bTrigg, GL_DYNAMIC_DRAW));

        foreach (const Trigger *trigger, m_Entities->triggers())
        {
            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(trigger->positionX*16+m_Translation.x(), trigger->positionY*16+m_Translation.y());
            m_Program.setUniformValue("uni_mvp", mat_mvp);

            glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
        }

        const float bSign[16] = { 0, 0, 0.75f, 0.0f, 16, 0, 1.0f, 0.0f, 16, 16, 1.0f, 1.0f, 0, 16, 0.75f, 1.0f };
        glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, bSign, GL_DYNAMIC_DRAW));

        foreach (const Sign *sign, m_Entities->signs())
        {
            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(sign->positionX*16+m_Translation.x(), sign->positionY*16+m_Translation.y());
            m_Program.setUniformValue("uni_mvp", mat_mvp);

            glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setEntities(Map *map)
    {
        m_Entities = &map->entities();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setMapView(AMEMapView *view)
    {
        m_MapView = view;
        m_Translation = view->mainPos();
        //view->setEntities(m_Entities->npcs());
        setMinimumSize(m_MapView->minimumSize());
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::freeGL()
    {
        /* In this case, we destroy nothing because
         * the data is independent from the map */

        if (!m_VAO.isCreated())
            return;


        m_Entities = NULL;
        m_MapView = NULL;
    }
}
