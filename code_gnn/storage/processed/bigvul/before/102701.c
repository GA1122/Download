void CCThreadProxy::commitOnCCThread(CCCompletionEvent* completion)
{
    TRACE_EVENT("CCThreadProxy::beginFrameAndCommitOnCCThread", this, 0);
    ASSERT(isImplThread());
    ASSERT(m_schedulerOnCCThread->commitPending());
    if (!m_layerTreeHostImpl) {
        completion->signal();
        return;
    }
    m_layerTreeHostImpl->beginCommit();
    m_layerTreeHost->commitToOnCCThread(m_layerTreeHostImpl.get());
    m_layerTreeHostImpl->commitComplete();

    completion->signal();

    m_schedulerOnCCThread->didCommit();
}
