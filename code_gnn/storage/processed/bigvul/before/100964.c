void TextureMapperGL::endPainting()
{
#if PLATFORM(QT)
    glClearStencil(1);
    glClear(GL_STENCIL_BUFFER_BIT);
    glUseProgram(m_data->previousProgram);
    if (!m_context)
        return;
    QPainter* painter = m_context->platformContext();
    painter->endNativePainting();
    painter->restore();
#endif
}
