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
#include <AME/Widgets/OpenGL/AMEBlockView.h>
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
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    AMEBlockView::AMEBlockView(QWidget *parent)
        : QOpenGLWidget(parent),
          QOpenGLFunctions(),
          m_Palettes(NULL),
          m_SelectedBlock(-1)
    {
        QSurfaceFormat format = this->format();
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CompatibilityProfile);
        setFormat(format);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    AMEBlockView::~AMEBlockView()
    {
        if (m_VAO.isCreated())
        {
            freeGL();

            makeCurrent();
            m_Program.removeAllShaders();
            m_VAO.destroy();
            doneCurrent();
        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::initializeGL()
    {
        if (qboy::GLErrors::Current == NULL)
            qboy::GLErrors::Current = new qboy::GLErrors;


        initializeOpenGLFunctions();       

        // Initializes the shader program
        m_Program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/BlockVertexShader.glsl");
        m_Program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/BlockFragmentShader.glsl");
        m_Program.link();
        m_Program.bind();
        m_Program.setUniformValue("smp_texture", 0);
        m_Program.setUniformValue("smp_palette", 1);
        
        // Initializes the primitive shader program
        m_PmtProg.create();
        m_PmtProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/PrimitiveVertexShader.glsl");
        m_PmtProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/PrimitiveFragmentShader.glsl");
        m_PmtProg.link();
        

        // Initializes the vao
        m_VAO.create();
        m_VAO.bind();

        // Enables blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::resizeGL(int w, int h)
    {
        glCheck(glViewport(0, 0, w, h));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::paintGL()
    {
        glCheck(glClearColor(240/255.0f, 240/255.0f, 240/255.0f, 1));
        glCheck(glClear(GL_COLOR_BUFFER_BIT));

        if (!m_Palettes)
            return;

        // Binds the vertex array and the index buffer
        glCheck(m_VAO.bind());
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));


        // Paints the primary background and foreground
        QMatrix4x4 mat_mvp;
        mat_mvp.setToIdentity();
        mat_mvp.ortho(0, width(), height(), 0, -1, 1);

        // Binds the primary vertex buffer
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers.at(0)));

        // Specifies the matrix and buffers within the shader program
        m_Program.bind();
        m_Program.setUniformValue("uni_mvp", mat_mvp);
        m_Program.setUniformValue("is_background", true);
        m_Program.enableAttributeArray(MV_VERTEX_ATTR);
        m_Program.enableAttributeArray(MV_COORD_ATTR);
        m_Program.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 4*sizeof(float));
        m_Program.setAttributeBuffer(MV_COORD_ATTR,  GL_FLOAT, 2*sizeof(float), 2, 4*sizeof(float));

        // Draws them
        glCheck(glActiveTexture(GL_TEXTURE1));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_PalTexture));
        glCheck(glActiveTexture(GL_TEXTURE0));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_Textures.at(0)));
        glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        m_Program.setUniformValue("is_background", false);
        glCheck(glBindTexture(GL_TEXTURE_2D, m_Textures.at(1)));
        glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));


        // Paints the secondary background and foreground
        mat_mvp.setToIdentity();
        mat_mvp.ortho(0, width(), height(), 0, -1, 1);
        mat_mvp.translate(0, m_PrimarySetSize.height());

        // Binds the secondary vertex buffer
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers.at(1)));

        // Specifies the matrix and buffers within the shader program
        m_Program.setUniformValue("uni_mvp", mat_mvp);
        m_Program.setUniformValue("is_background", true);
        m_Program.enableAttributeArray(MV_VERTEX_ATTR);
        m_Program.enableAttributeArray(MV_COORD_ATTR);
        m_Program.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 4*sizeof(float));
        m_Program.setAttributeBuffer(MV_COORD_ATTR,  GL_FLOAT, 2*sizeof(float), 2, 4*sizeof(float));

        // Draws them
        glCheck(glActiveTexture(GL_TEXTURE0));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_Textures.at(2)));
        glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        m_Program.setUniformValue("is_background", false);
        glCheck(glBindTexture(GL_TEXTURE_2D, m_Textures.at(3)));
        glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
        
        
        // Draw selection rectangle
        if (m_SelectedBlock != -1)
        {
            // Defines the uniform vertex attributes
            static const float vertRect[8]
            {
                0.5f,  0.5f,
                15.5f, 0.5f,
                15.5f, 15.5f,
                0.5f,  15.5f
            };
            
            
            // Computes the position on-widget
            float x = (m_SelectedBlock % 8) * 16;
            float y = (m_SelectedBlock / 8) * 16;
            
            unsigned rectBuffer = 0;
            glCheck(glGenBuffers(1, &rectBuffer));
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, rectBuffer));
            
            
            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(x, y);
            
            // Modifies program states
            m_PmtProg.bind();
            m_PmtProg.enableAttributeArray(MV_VERTEX_ATTR);
            m_PmtProg.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 2*sizeof(float));
            m_PmtProg.setUniformValue("uni_color", QVector4D(1.0f, 0.0f, 0.0f, 0.8f));
            m_PmtProg.setUniformValue("uni_mvp", mat_mvp);
            
            // Perform rendering
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, rectBuffer));
            glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, vertRect, GL_STATIC_DRAW));
            glCheck(glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, NULL));
            glCheck(glDeleteBuffers(1, &rectBuffer));
        }
    }
    
    
    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/25/2016
    //
    ///////////////////////////////////////////////////////////
    int AMEBlockView::selectedBlock() const
    {
        return m_SelectedBlock;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::setMapView(AMEMapView *view)
    {
        makeCurrent();
        initializeOpenGLFunctions();

        // Retrieves necessary members from AMEMapView
        QList<UInt8 *> pixels = view->mainPixels();
        m_Palettes = view->mainPalettes();
        m_PrimaryBackground = pixels[0];
        m_PrimaryForeground = pixels[1];
        m_SecondaryBackground = pixels[2];
        m_SecondaryForeground = pixels[3];
        m_PrimarySetSize = view->primarySetSize();
        m_SecondarySetSize = view->secondarySetSize();
        m_VAO.bind();

        // Creates, bind and buffers the static index buffer
        const unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
        glCheck(glGenBuffers(1, &m_IndexBuffer));
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
        glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*6, indices, GL_STATIC_READ));

        // Creates one vertex buffer for each blockset
        float wid1 = (float)m_PrimarySetSize.width();
        float hei1 = (float)m_PrimarySetSize.height();
        float wid2 = (float)m_SecondarySetSize.width();
        float hei2 = (float)m_SecondarySetSize.height();
        float buf1[16] =
        {
        //  X     Y       U   V
            0,    0,      0,  0,
            wid1, 0,      1,  0,
            wid1, hei1,   1,  1,
            0,    hei1,   0,  1
        };
        float buf2[16] =
        {
        //  X     Y       U   V
            0,    0,      0,  0,
            wid2, 0,      1,  0,
            wid2, hei2,   1,  1,
            0,    hei2,   0,  1
        };

        // Generates the buffers and stores the vertices
        unsigned vcs1 = 0, vcs2 = 0;
        glCheck(glGenBuffers(1, &vcs1));
        glCheck(glGenBuffers(1, &vcs2));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vcs1));
        glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, buf1, GL_DYNAMIC_DRAW));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vcs2));
        glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, buf2, GL_DYNAMIC_DRAW));
        m_VertexBuffers.push_back(vcs1);
        m_VertexBuffers.push_back(vcs2);


        // Creates foreground and background textures
        unsigned array[] = { 0, 0, 0, 0 };
        glCheck(glGenTextures(4, array));
        glCheck(glBindTexture(GL_TEXTURE_2D, array[0]));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)wid1, (int)hei1, 0, GL_RED, GL_UNSIGNED_BYTE, m_PrimaryBackground));
        glCheck(glBindTexture(GL_TEXTURE_2D, array[1]));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)wid1, (int)hei1, 0, GL_RED, GL_UNSIGNED_BYTE, m_PrimaryForeground));
        glCheck(glBindTexture(GL_TEXTURE_2D, array[2]));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)wid2, (int)hei2, 0, GL_RED, GL_UNSIGNED_BYTE, m_SecondaryBackground));
        glCheck(glBindTexture(GL_TEXTURE_2D, array[3]));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)wid2, (int)hei2, 0, GL_RED, GL_UNSIGNED_BYTE, m_SecondaryForeground));
        for (int i = 0; i < 4; i++)
            m_Textures.push_back(array[i]);

        // Creates one palette texture for all blocksets
        glCheck(glGenTextures(1, &m_PalTexture));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_PalTexture));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1, 0, GL_RGBA, GL_FLOAT, m_Palettes->data()));


        // Sets the minimum size
        setMinimumWidth(m_PrimarySetSize.width());
        setMinimumHeight(m_PrimarySetSize.height() + m_SecondarySetSize.height());
        doneCurrent();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::freeGL()
    {
        if (!m_VAO.isCreated())
            return;

        makeCurrent();


        foreach (UInt32 id, m_Textures)
            glCheck(glDeleteTextures(1, &id));
        foreach (UInt32 id, m_VertexBuffers)
            glCheck(glDeleteBuffers(1, &id));

        glCheck(glDeleteTextures(1, &m_PalTexture));
        glCheck(glDeleteBuffers(1, &m_IndexBuffer));

        m_Palettes = NULL;
        m_Textures.clear();
        m_VertexBuffers.clear();


        doneCurrent();
        repaint();
    }
    
    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/25/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::mouseReleaseEvent(QMouseEvent *event)
    {
        int mouseX = event->pos().x();
        int mouseY = event->pos().y();
        if (mouseX >= width() || mouseY >= height())
        {
            m_SelectedBlock = -1;
            repaint();
            return;
        }
        
        // Align the position to a multiple of 16px
        while (mouseX % 16 != 0)
            mouseX--;
        while (mouseY % 16 != 0)
            mouseY--;
        
        // Determines the tile-number
        m_SelectedBlock = ((mouseY/16)*8) + (mouseX/16);
        repaint();
    }
}
