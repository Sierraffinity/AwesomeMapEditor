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
          m_SelectedBlocks({ 0 }),
          m_FirstBlock(0),
          m_LastBlock(0),
          m_CurrentTool(AMEMapView::Tool::None),
          m_CursorColor(Qt::GlobalColor::red),
          m_ShowHighlight(false)
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
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::copyBlocks(AMEBlockView *other)
    {
        other->m_VertexBuffers = m_VertexBuffers;
        other->m_Textures = m_Textures;
        other->m_PalTexture = m_PalTexture;
        other->m_IndexBuffer = m_IndexBuffer;
        other->m_Palettes = m_Palettes;
        other->m_PrimarySetSize = m_PrimarySetSize;
        other->m_SecondarySetSize = m_SecondarySetSize;
        other->m_PrimaryForeground = m_PrimaryForeground;
        other->m_PrimaryBackground = m_PrimaryBackground;
        other->m_SecondaryBackground = m_SecondaryBackground;
        other->m_SecondaryForeground = m_SecondaryForeground;
        other->m_SelectedBlocks = m_SelectedBlocks;
        other->m_FirstBlock = m_FirstBlock;
        other->m_LastBlock = m_LastBlock;
        other->m_CurrentTool = m_CurrentTool;
        other->m_CursorColor = m_CursorColor;
        other->m_ShowHighlight = m_ShowHighlight;
        other->setMinimumSize(minimumSize());
        other->resize(minimumSize());
        other->repaint();
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

        m_PmtProg.bind();

        // Draw selection rectangle
        if (m_FirstBlock != -1 && m_LastBlock != -1)
        {
            // Computes the position on widget based on selected blocks
            int firstX = (m_FirstBlock % 8);
            int x = firstX;
            int firstY = (m_FirstBlock / 8);
            int y = firstY;

            int lastX = (m_LastBlock % 8);
            int lastY = (m_LastBlock / 8);

            int selectorWidth = lastX - firstX;
            int selectorHeight = lastY - firstY;

            if (lastX < firstX)
            {
                x = lastX;
                selectorWidth = firstX - x;
            }

            if (lastY < firstY)
            {
                y = lastY;
                selectorHeight = firstY - y;
            }

            // This should be made user-changable later
            x *= 16;
            y *= 16;
            selectorWidth *= 16;
            selectorHeight *= 16;

            // Defines the uniform vertex attributes
            float vertRect[8]
            {
                0.5f,                0.5f,
                selectorWidth+15.5f, 0.5f,
                selectorWidth+15.5f, selectorHeight+15.5f,
                0.5f,                selectorHeight+15.5f
            };

            unsigned rectBuffer = 0;
            glCheck(glGenBuffers(1, &rectBuffer));
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, rectBuffer));

            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(x, y);

            // Modifies program states
            m_PmtProg.enableAttributeArray(MV_VERTEX_ATTR);
            m_PmtProg.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 2*sizeof(float));
            m_PmtProg.setUniformValue("uni_color", m_CursorColor);
            m_PmtProg.setUniformValue("uni_mvp", mat_mvp);

            // Render current selected block rect
            //glCheck(glBindBuffer(GL_ARRAY_BUFFER, rectBuffer));
            glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, vertRect, GL_STATIC_DRAW));
            glCheck(glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, NULL));
            glCheck(glDeleteBuffers(1, &rectBuffer));
        }

        if (m_ShowHighlight)
        {
            static const float highlightRect[8]
            {
                0.5f,  0.5f,
                15.5f, 0.5f,
                15.5f, 15.5f,
                0.5f,  15.5f
            };

            int highlightX = (m_HighlightedBlock % 8) * 16;
            int highlightY = (m_HighlightedBlock / 8) * 16;

            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(highlightX, highlightY);

            // Render highlighted block rect
            unsigned highlightBuffer = 0;
            glCheck(glGenBuffers(1, &highlightBuffer));
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, highlightBuffer));

            // Modifies program states
            m_PmtProg.enableAttributeArray(MV_VERTEX_ATTR);
            m_PmtProg.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 2*sizeof(float));
            m_PmtProg.setUniformValue("uni_color", QColor(Qt::GlobalColor::green));
            m_PmtProg.setUniformValue("uni_mvp", mat_mvp);

            //glCheck(glBindBuffer(GL_ARRAY_BUFFER, highlightBuffer));
            glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, highlightRect, GL_STATIC_DRAW));
            glCheck(glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, NULL));
            glCheck(glDeleteBuffers(1, &highlightBuffer));
        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/4/2016
    //
    ///////////////////////////////////////////////////////////
    QVector<UInt16> AMEBlockView::selectedBlocks()
    {
        return m_SelectedBlocks;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::selectBlock(UInt16 newBlock)
    {
        m_FirstBlock = newBlock;
        m_LastBlock = newBlock;
        m_SelectedBlocks = { newBlock };
        static_cast<QScrollArea>(parentWidget()).ensureVisible(0, (newBlock / 8) * 16);  // TODO: Does not scroll correctly, probably wrong parent
        repaint();
        return;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::deselectBlocks()
    {
        m_FirstBlock = -1;
        m_LastBlock = -1;
        m_SelectedBlocks.clear();
        return;
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
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/4/2016
    //
    ///////////////////////////////////////////////////////////
    AMEMapView::Tool AMEBlockView::getCurrentTool(Qt::MouseButtons buttons)
    {
        if (buttons & Qt::LeftButton)
        {
            if (m_CurrentTool == AMEMapView::Tool::Select)
                return AMEMapView::Tool::Select;
            return AMEMapView::Tool::Draw;
        }
        else if (buttons & Qt::RightButton)
        {
            if (m_CurrentTool == AMEMapView::Tool::Draw)
                return AMEMapView::Tool::Draw;
            return AMEMapView::Tool::Select;
        }
        return AMEMapView::Tool::None;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/4/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::setCurrentTool(AMEMapView::Tool newTool)
    {
        m_CurrentTool = newTool;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/4/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::mousePressEvent(QMouseEvent *event)
    {
        m_ShowHighlight = false;
        AMEMapView::Tool currentTool = getCurrentTool(event->buttons());

        int mouseX = event->pos().x();
        int mouseY = event->pos().y();
        if (mouseX < 0 || mouseY < 0 ||
                mouseX >= width() || mouseY >= height())
        {
            return;
        }

        // Determines the block number
        m_FirstBlock = (mouseX/16) + ((mouseY/16)*8);
        m_LastBlock = m_FirstBlock;

        if (currentTool == AMEMapView::Tool::Select)
            m_CursorColor = Qt::GlobalColor::yellow;

        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/5/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
        m_ShowHighlight = true;
        m_CursorColor = Qt::GlobalColor::red;

        // Determines the tile-number
        int selectionWidth = (m_LastBlock % 8) - (m_FirstBlock % 8);
        if (selectionWidth < 0)
        {
            selectionWidth = -selectionWidth;
            m_FirstBlock -= selectionWidth;
            m_LastBlock += selectionWidth;
        }
        selectionWidth++;

        int selectionHeight = (m_LastBlock / 8) - (m_FirstBlock / 8);
        if (selectionHeight < 0)
        {
            selectionHeight = -selectionHeight;
            m_FirstBlock -= selectionHeight * 8;
            m_LastBlock += selectionHeight * 8;
        }
        selectionHeight++;

        m_SelectedBlocks.clear();

        for (int i = 0; i < selectionHeight; i++)
        {
            for (int j = 0; j < selectionWidth; j++)
            {
                m_SelectedBlocks.push_back(m_FirstBlock + j + (i * 8));
            }
        }
        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::mouseMoveEvent(QMouseEvent *event)
    {
        int mouseX = event->pos().x();
        int mouseY = event->pos().y();
        if (mouseX < 0)
            mouseX = 0;
        else if (mouseY < 0)
            mouseY = 0;
        else if (mouseX >= width())
            mouseX = width();
        else if (mouseY >= height())
            mouseY = height();

        AMEMapView::Tool currentTool = getCurrentTool(event->buttons());

        if (currentTool == AMEMapView::Tool::Draw)
        {
            // Determines the block number
            m_FirstBlock = (mouseX/16) + ((mouseY/16)*8);
            m_LastBlock = m_FirstBlock;
        }
        else if (currentTool == AMEMapView::Tool::Select)
        {
            // Determines the block number
            m_LastBlock = (mouseX/16) + ((mouseY/16)*8);
        }

        m_HighlightedBlock = (mouseX/16) + ((mouseY/16)*8);

        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::enterEvent(QEvent *event)
    {
        Q_UNUSED(event);
        m_ShowHighlight = true;

        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::leaveEvent(QEvent *event)
    {
        Q_UNUSED(event);
        m_ShowHighlight = false;

        repaint();
    }
}
