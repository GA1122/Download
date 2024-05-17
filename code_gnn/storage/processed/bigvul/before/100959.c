void BitmapTextureGL::destroy()
{
    if (m_id && (!m_imageUID || !m_textureMapper->data().directlyCompositedImages.deref(m_imageUID)))
        GL_CMD(glDeleteTextures(1, &m_id))

    if (m_fbo)
        GL_CMD(glDeleteFramebuffers(1, &m_fbo))

    if (m_rbo)
        GL_CMD(glDeleteRenderbuffers(1, &m_rbo))

    m_fbo = 0;
    m_id = 0;
    m_textureSize = IntSize();
    m_relativeSize = FloatSize(1, 1);
}
