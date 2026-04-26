#ifndef BALLVIEWPORT_H
#define BALLVIEWPORT_H

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
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
};

#endif
