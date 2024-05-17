void CCThreadProxy::drawLayersOnCCThread()
{
    TRACE_EVENT("CCThreadProxy::drawLayersOnCCThread", this, 0);
    ASSERT(isImplThread());
    ASSERT(m_layerTreeHostImpl);

    m_layerTreeHostImpl->drawLayers();
    ASSERT(!m_layerTreeHostImpl->isContextLost());
}
