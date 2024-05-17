PassOwnPtr<CCMainThread::Task> CCThreadProxy::createBeginFrameAndCommitTaskOnCCThread()
{
    TRACE_EVENT("CCThreadProxy::createBeginFrameAndCommitTaskOnCCThread", this, 0);
    ASSERT(isImplThread());
    double frameBeginTime = currentTime();

    int thisTaskSequenceNumber = m_numBeginFrameAndCommitsIssuedOnCCThread;
    m_numBeginFrameAndCommitsIssuedOnCCThread++;
    OwnPtr<CCScrollUpdateSet> scrollInfo = m_layerTreeHostImpl->processScrollDeltas();
    return createMainThreadTask(this, &CCThreadProxy::beginFrameAndCommit, thisTaskSequenceNumber, frameBeginTime, scrollInfo.release());
}
