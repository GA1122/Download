void BitmapTextureGL::setContentsToImage(Image* image)
{
    ImageUID uid = image ? uidForImage(image) : 0;
    if (!image || !uid) {
        if (m_imageUID)
            destroy();
        return;
    }

    if (uid == m_imageUID)
        return;
    bool found = false;
    GLuint newTextureID = m_textureMapper->data().directlyCompositedImages.findOrCreate(uid, found);
    if (newTextureID != m_id) {
        m_imageUID = uid;
        destroy();
        m_id = newTextureID;
        reset(image->size(), false);
        if (!found) {
            GraphicsContext context(beginPaint(IntRect(0, 0, m_textureSize.width(), m_textureSize.height())));
            context.drawImage(image, ColorSpaceDeviceRGB, IntPoint(0, 0), CompositeCopy);
            endPaint();
        }
    }
}
