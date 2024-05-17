void CCThreadProxy::drawLayersAndPresentOnCCThread()
{
    TRACE_EVENT("CCThreadProxy::drawLayersOnCCThread", this, 0);
    ASSERT(isImplThread());
    if (!m_layerTreeHostImpl)
        return;

    drawLayersOnCCThread();
    m_layerTreeHostImpl->present();
    m_schedulerOnCCThread->didDraw();
}
