void CCThreadProxy::beginFrameAndCommit(int sequenceNumber, double frameBeginTime, PassOwnPtr<CCScrollUpdateSet> scrollInfo)
{
    TRACE_EVENT("CCThreadProxy::beginFrameAndCommit", this, 0);
    ASSERT(isMainThread());
    if (!m_layerTreeHost)
        return;

    m_layerTreeHost->applyScrollDeltas(*scrollInfo.get());

    if (sequenceNumber < m_lastExecutedBeginFrameAndCommitSequenceNumber) {
        TRACE_EVENT("EarlyOut_StaleBeginFrameAndCommit", this, 0);
        return;
    }
    m_lastExecutedBeginFrameAndCommitSequenceNumber = sequenceNumber;

    {
        TRACE_EVENT("CCLayerTreeHost::animateAndLayout", this, 0);
        m_layerTreeHost->animateAndLayout(frameBeginTime);
    }

    ASSERT(m_lastExecutedBeginFrameAndCommitSequenceNumber == sequenceNumber);

    m_commitRequested = false;

    m_layerTreeHost->updateLayers();

    {
        TRACE_EVENT("commit", this, 0);
        CCCompletionEvent completion;
        s_ccThread->postTask(createCCThreadTask(this, &CCThreadProxy::commitOnCCThread, AllowCrossThreadAccess(&completion)));
        completion.wait();
    }

    m_layerTreeHost->commitComplete();

    if (m_redrawAfterCommit)
        setNeedsRedraw();
    m_redrawAfterCommit = false;

    ASSERT(m_lastExecutedBeginFrameAndCommitSequenceNumber == sequenceNumber);
}
