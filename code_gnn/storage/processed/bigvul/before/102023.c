void GraphicsSurface::platformCopyFromTexture(uint32_t texture, const IntRect& sourceRect)
{
    m_private->copyFromTexture(texture, sourceRect);
}
