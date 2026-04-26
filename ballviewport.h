#ifndef BALLVIEWPORT_H
#define BALLVIEWPORT_H

#define GL_SILENCE_DEPRECATION

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class BallViewport : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit BallViewport(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    GLuint m_shaderProgram = 0;

    // For the bounding box
    GLuint m_boxVao = 0;
    GLuint m_boxVbo = 0;

    // For the spheres
    GLuint m_sphereVao = 0;
    GLuint m_sphereVbo = 0;
    GLuint m_sphereEbo = 0;
    int m_sphereIndexCount = 0;


};

#endif
