void CCThreadProxy::finishAllRenderingOnCCThread(CCCompletionEvent* completion)
{
    TRACE_EVENT("CCThreadProxy::finishAllRenderingOnCCThread", this, 0);
    ASSERT(isImplThread());
    if (m_schedulerOnCCThread->redrawPending()) {
        drawLayersOnCCThread();
        m_layerTreeHostImpl->present();
        m_schedulerOnCCThread->didDraw();
    }
    m_layerTreeHostImpl->finishAllRendering();
     completion->signal();
 }
