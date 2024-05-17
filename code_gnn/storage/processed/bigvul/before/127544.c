void LayerWebKitThread::updateTextureContentsIfNeeded()
{
    if (m_tiler)
        m_tiler->updateTextureContentsIfNeeded(m_isMask ? 1.0 : contentsScale());
}
