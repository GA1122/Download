void CCLayerTreeHost::commitComplete()
{
    clearPendingUpdate();
    m_contentsTextureManager->unprotectAllTextures();
}
