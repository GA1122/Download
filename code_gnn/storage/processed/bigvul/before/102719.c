void CCThreadProxy::layerTreeHostClosedOnCCThread(CCCompletionEvent* completion)
{
    TRACE_EVENT("CCThreadProxy::layerTreeHostClosedOnCCThread", this, 0);
    ASSERT(isImplThread());
    m_layerTreeHost->deleteContentsTexturesOnCCThread(m_layerTreeHostImpl->contentsTextureAllocator());
    m_layerTreeHostImpl.clear();
    m_inputHandlerOnCCThread.clear();
    m_scrollControllerAdapterOnCCThread.clear();
    completion->signal();
}
