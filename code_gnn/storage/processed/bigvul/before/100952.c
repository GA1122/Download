void BitmapTextureGL::bind()
{
    int& stencilIndex = TextureMapperGLData::globalGLData.stencilIndex;
    if (m_surfaceNeedsReset || !m_fbo) {
        if (!m_fbo)
            GL_CMD(glGenFramebuffers(1, &m_fbo))
        if (!m_rbo)
            GL_CMD(glGenRenderbuffers(1, &m_rbo));
        GL_CMD(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo))
#ifdef TEXMAP_OPENGL_ES_2
        GL_CMD(glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, m_textureSize.width(), m_textureSize.height()))
#else
        GL_CMD(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, m_textureSize.width(), m_textureSize.height()))
#endif
        GL_CMD(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo))
        GL_CMD(glBindTexture(GL_TEXTURE_2D, 0))
        GL_CMD(glBindRenderbuffer(GL_RENDERBUFFER, 0))
        GL_CMD(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id(), 0))
        GL_CMD(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo))
#ifndef TEXMAP_OPENGL_ES_2
        GL_CMD(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo));
#endif
        GL_CMD(glClearColor(0, 0, 0, 0))
        GL_CMD(glClearStencil(stencilIndex - 1))
        GL_CMD(glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
        m_surfaceNeedsReset = false;
    } else
        GL_CMD(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo))

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(stencilIndex > 1 ? GL_GEQUAL : GL_ALWAYS, stencilIndex - 1, stencilIndex - 1);
    GL_CMD(glViewport(0, 0, size().width(), size().height()))
    m_textureMapper->data().projectionMatrix = createProjectionMatrix(size(), false);
    glDisable(GL_SCISSOR_TEST);
}
