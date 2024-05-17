void BitmapTextureGL::reset(const IntSize& newSize, bool opaque)
{
    BitmapTexture::reset(newSize, opaque);
    m_imageUID = 0;
    IntSize newTextureSize = nextPowerOfTwo(newSize);
    bool justCreated = false;
    if (!m_id) {
        GL_CMD(glGenTextures(1, &m_id))
        justCreated = true;
    }

    if (justCreated || newTextureSize.width() > m_textureSize.width() || newTextureSize.height() > m_textureSize.height()) {
        m_textureSize = newTextureSize;
        GL_CMD(glBindTexture(GL_TEXTURE_2D, m_id))
        GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
        GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
        GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
        GL_CMD(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
        texImage2DResourceSafe(m_textureSize.width(), m_textureSize.height());
    }
    m_actualSize = newSize;
    m_relativeSize = FloatSize(float(newSize.width()) / m_textureSize.width(), float(newSize.height()) / m_textureSize.height());
    m_opaque = opaque;
    m_surfaceNeedsReset = true;
}
