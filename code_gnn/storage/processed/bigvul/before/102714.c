void CCThreadProxy::initializeImplOnCCThread(CCCompletionEvent* completion)
{
    TRACE_EVENT("CCThreadProxy::initializeImplOnCCThread", this, 0);
    ASSERT(isImplThread());
    m_layerTreeHostImpl = m_layerTreeHost->createLayerTreeHostImpl();
    m_schedulerClientOnCCThread = CCThreadProxySchedulerClient::create(this);
    m_schedulerOnCCThread = CCScheduler::create(m_schedulerClientOnCCThread.get());
    completion->signal();
}
