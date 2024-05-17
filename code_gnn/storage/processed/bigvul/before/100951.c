void TextureMapperGL::beginPainting()
{
#if PLATFORM(QT)
    if (!initializeOpenGLShims())
        return;

    glGetIntegerv(GL_CURRENT_PROGRAM, &m_data->previousProgram);
    if (m_context) {
        QPainter* painter = m_context->platformContext();
        painter->save();
        painter->beginNativePainting();
    }
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    bindSurface(0);
#endif
    initializeShaders();
}
