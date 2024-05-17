void CCLayerTreeHost::commitToOnCCThread(CCLayerTreeHostImpl* hostImpl)
{
    ASSERT(CCProxy::isImplThread());
    TRACE_EVENT("CCLayerTreeHost::commitTo", this, 0);
    hostImpl->setSourceFrameNumber(frameNumber());

    contentsTextureManager()->reduceMemoryToLimit(TextureManager::reclaimLimitBytes());
    contentsTextureManager()->deleteEvictedTextures(hostImpl->contentsTextureAllocator());

    updateCompositorResources(m_updateList, hostImpl->context(), hostImpl->contentsTextureAllocator());

    hostImpl->setVisible(m_visible);
    hostImpl->setZoomAnimatorTransform(m_zoomAnimatorTransform);
    hostImpl->setViewport(viewportSize());

    if (rootLayer())
        hostImpl->setRootLayer(TreeSynchronizer::synchronizeTrees(rootLayer(), hostImpl->rootLayer()));
    else
        hostImpl->setRootLayer(0);

    m_frameNumber++;
}
