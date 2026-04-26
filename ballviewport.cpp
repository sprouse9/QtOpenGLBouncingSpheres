#include "ballviewport.h"

BallViewport::BallViewport(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void BallViewport::initializeGL()
{
    qDebug() << "initializeGL called";

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    qDebug() << "OpenGL version:" << reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << "GLSL version:" << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    static const char *vertexShaderSource =
        "#version 410 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "}";

    static const char *fragmentShaderSource =
        "#version 410 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        qDebug() << "Vertex shader compile failed:" << infoLog;
    }
    else{
        qDebug() << "Vertex shader compiled successfully";
    }

    // Now we compile the FRAGMENT SHADER
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    success = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        qDebug() << "Fragment shader compile failed:" << infoLog;
    }
    else
    {
        qDebug() << "Fragment shader compiled successfully";
    }

    // Now we link
    m_shaderProgram = glCreateProgram();

    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);

    glLinkProgram(m_shaderProgram);

    //success = 0;
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        qDebug() << "Shader program link failed:" << infoLog;
    }
    else
    {
        qDebug() << "Shader program linked successfully";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float boxVertices[] = {
        // front face z = 0.8
        -0.8f, -0.8f,  0.8f,   0.8f, -0.8f,  0.8f,
        0.8f, -0.8f,  0.8f,   0.8f,  0.8f,  0.8f,
        0.8f,  0.8f,  0.8f,  -0.8f,  0.8f,  0.8f,
        -0.8f,  0.8f,  0.8f,  -0.8f, -0.8f,  0.8f,

        // back face z = -0.8
        -0.8f, -0.8f, -0.8f,   0.8f, -0.8f, -0.8f,
        0.8f, -0.8f, -0.8f,   0.8f,  0.8f, -0.8f,
        0.8f,  0.8f, -0.8f,  -0.8f,  0.8f, -0.8f,
        -0.8f,  0.8f, -0.8f,  -0.8f, -0.8f, -0.8f,

        // connecting edges
        -0.8f, -0.8f,  0.8f,  -0.8f, -0.8f, -0.8f,
        0.8f, -0.8f,  0.8f,   0.8f, -0.8f, -0.8f,
        0.8f,  0.8f,  0.8f,   0.8f,  0.8f, -0.8f,
        -0.8f,  0.8f,  0.8f,  -0.8f,  0.8f, -0.8f
    };

    glGenVertexArrays(1, &m_boxVao);
    glGenBuffers(1, &m_boxVbo);

    glBindVertexArray(m_boxVao);

    glBindBuffer(GL_ARRAY_BUFFER, m_boxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // location = 0 in shader
        3,                  // vec3
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
        );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Add the sphere's buffers
    glGenVertexArrays(1, &m_sphereVao);
    glGenBuffers(1, &m_sphereVbo);
    glGenBuffers(1, &m_sphereEbo);

    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;

    const float radius = 0.25f;
    const int stacks = 16;
    const int sectors = 32;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float theta = stack * M_PI / stacks;   // 0 to pi

        for (int sector = 0; sector <= sectors; ++sector)
        {
            float phi = sector * 2.0f * M_PI / sectors; // 0 to 2pi

            float x = radius * sin(theta) * cos(phi);
            float y = radius * cos(theta);
            float z = radius * sin(theta) * sin(phi);

            sphereVertices.push_back(x);
            sphereVertices.push_back(y);
            sphereVertices.push_back(z);
        }
    }

    for (int stack = 0; stack < stacks; ++stack)
    {
        for (int sector = 0; sector < sectors; ++sector)
        {
            int first = stack * (sectors + 1) + sector;
            int second = first + sectors + 1;

            sphereIndices.push_back(first);
            sphereIndices.push_back(second);
            sphereIndices.push_back(first + 1);

            sphereIndices.push_back(second);
            sphereIndices.push_back(second + 1);
            sphereIndices.push_back(first + 1);
        }
    }

    m_sphereIndexCount = static_cast<int>(sphereIndices.size());



    qDebug() << "Sphere vertices:" << sphereVertices.size() / 3;
    qDebug() << "Sphere indices:" << sphereIndices.size();
    qDebug() << "m_sphereIndexCount:" << m_sphereIndexCount;


    glBindVertexArray(m_sphereVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphereVbo);

    glBufferData(GL_ARRAY_BUFFER,
                 sphereVertices.size() * sizeof(float),
                 sphereVertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sphereIndices.size() * sizeof(unsigned int),
                 sphereIndices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);



}

void BallViewport::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void BallViewport::paintGL()
{
    glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shaderProgram);


    glBindVertexArray(m_boxVao);
    glDrawArrays(GL_LINES, 0, 24);


    glBindVertexArray(m_sphereVao);
    glDrawElements(GL_TRIANGLES, m_sphereIndexCount, GL_UNSIGNED_INT, nullptr);


    glBindVertexArray(0);

}