void GraphicsSurface::platformDestroy()
{
    if (m_texture) {
        pGlXReleaseTexImageEXT(m_private->display(), m_private->glxPixmap(), GLX_FRONT_EXT);
        glDeleteTextures(1, &m_texture);
    }

    delete m_private;
    m_private = 0;
}
