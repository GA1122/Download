PlatformGraphicsContext* BitmapTextureGL::beginPaint(const IntRect& dirtyRect)
{
    m_buffer = BGRA32PremultimpliedBuffer::create();
    m_dirtyRect = dirtyRect;
    return m_buffer->beginPaint(dirtyRect, m_opaque);
}
