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
        m_PmtProg.create();
        m_PmtProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/PrimitiveVertexShader.glsl");
        m_PmtProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/PrimitiveFragmentShader.glsl");
        m_PmtProg.link();

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
        QMatrix4x4 mat_mvp;


        // Draws the movement radius bounds
        if (m_Selection.type == ET_Npc)
        {
            // Creates a temporary buffer
            unsigned boundsBuffer = 0;
            glCheck(glGenBuffers(1, &boundsBuffer));
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, boundsBuffer));

            // Sets up the primitives program
            m_PmtProg.bind();
            m_PmtProg.enableAttributeArray(MV_VERTEX_ATTR);
            m_PmtProg.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 2*sizeof(float));
            m_PmtProg.setUniformValue("uni_color", QVector4D(1.0f, 0.0f, 1.0f, 0.2f));
            m_PmtProg.setUniformValue("uni_mvp", mat_mvp);


            Npc *currentNpc = (Npc *) m_Selection.entity;
            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(
                (m_Translation.x()+currentNpc->positionX*16)-currentNpc->moveRadiusX*16,
                (m_Translation.y()+currentNpc->positionY*16)-currentNpc->moveRadiusY*16
            );

            m_PmtProg.setUniformValue("uni_mvp", mat_mvp);

            float npcBounds[8] { 0, 0, currentNpc->moveRadiusX*32+16.0f, 0, currentNpc->moveRadiusX*32+16.0f, currentNpc->moveRadiusY*32+16.0f, 0, currentNpc->moveRadiusY*32+16.0f };
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, boundsBuffer));
            glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, npcBounds, GL_STATIC_DRAW));
            glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
            glCheck(m_PmtProg.setUniformValue("uni_color", QVector4D(1.0f, 0.0f, 1.0f, 0.4f)));
            glCheck(glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, NULL));
            glCheck(glDeleteBuffers(1, &boundsBuffer));
        }



        // Binds the program and associates some properties
        //m_PmtProg.release();
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
        m_Program.bind();
        m_Program.enableAttributeArray(MV_VERTEX_ATTR);
        m_Program.enableAttributeArray(MV_COORD_ATTR);
        m_Program.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 4*sizeof(float));
        m_Program.setAttributeBuffer(MV_COORD_ATTR,  GL_FLOAT, 2*sizeof(float), 2, 4*sizeof(float));

        // Iterates through the entities and draws them
        const float bNpc[16] = { 0, 0, 0.0f, 0.0f, 16, 0, 0.25f, 0.0f, 16, 16, 0.25f, 1.0f, 0, 16, 0.0f, 1.0f };
        glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, bNpc, GL_DYNAMIC_DRAW));

        foreach (const Npc *npc, m_Entities->npcs())
        {
            // Draws the NPC field
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


        // Draws the selection square
        if (m_Selection.entity)
        {
            float posX = (float)m_Selection.absPos.x();
            float posY = (float)m_Selection.absPos.y();
            float buffer[8] =
            {
                0,  0,
                16, 0,
                16, 16,
                0,  16
            };

            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(posX, posY);

            unsigned tempBuffer;
            glCheck(glGenBuffers(1, &tempBuffer));
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, tempBuffer));
            glCheck(glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), buffer, GL_STATIC_DRAW));
            glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            glCheck(glBindTexture(GL_TEXTURE_2D, 0));

            m_PmtProg.bind();
            m_PmtProg.enableAttributeArray(MV_VERTEX_ATTR);
            m_PmtProg.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 2*sizeof(float));
            m_PmtProg.setUniformValue("uni_color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
            m_PmtProg.setUniformValue("uni_mvp", mat_mvp);

            glCheck(glDrawArrays(GL_LINE_LOOP, 0, 4));
            glCheck(glDeleteBuffers(1, &tempBuffer));
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::mousePressEvent(QMouseEvent *event)
    {
        emit onMouseClick(event);
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
        m_MapView->resize(m_MapView->minimumSize());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   1/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setCurrentEntity(CurrentEntity entity)
    {
        m_Selection = entity;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   1/7/2016
    //
    ///////////////////////////////////////////////////////////
    const CurrentEntity &AMEEntityView::currentEntity() const
    {
        return m_Selection;
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


        m_Selection = CurrentEntity();
        m_Entities = NULL;
        m_MapView = NULL;
    }
}
