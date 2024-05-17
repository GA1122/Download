void CCLayerTreeHost::setVisible(bool visible)
{
    m_visible = visible;
    if (visible)
        setNeedsCommitThenRedraw();
    else {
        m_contentsTextureManager->reduceMemoryToLimit(TextureManager::lowLimitBytes());
        m_contentsTextureManager->unprotectAllTextures();
        m_proxy->setNeedsCommit();
    }
}
