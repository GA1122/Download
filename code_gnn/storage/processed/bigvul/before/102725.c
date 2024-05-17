void CCThreadProxy::setNeedsCommitOnCCThread()
{
    ASSERT(isImplThread());
    TRACE_EVENT("CCThreadProxy::setNeedsCommitOnCCThread", this, 0);
    m_schedulerOnCCThread->requestCommit();
}
