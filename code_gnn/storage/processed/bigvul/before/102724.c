void CCThreadProxy::setNeedsCommit()
{
    ASSERT(isMainThread());
    if (m_commitRequested)
        return;

    TRACE_EVENT("CCThreadProxy::setNeedsCommit", this, 0);
    m_commitRequested = true;
    s_ccThread->postTask(createCCThreadTask(this, &CCThreadProxy::setNeedsCommitOnCCThread));
}
