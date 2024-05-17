void BitmapTextureGL::endPaint()
{
    if (!m_buffer)
        return;
    m_buffer->endPaint();
    updateContents(BGRAFormat, m_dirtyRect, m_buffer->data());
    GL_CMD(glBindTexture(GL_TEXTURE_2D, m_id))
    m_buffer.clear();
}
