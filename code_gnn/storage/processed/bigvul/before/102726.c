void CCThreadProxy::setNeedsCommitThenRedraw()
{
    ASSERT(isMainThread());
    m_redrawAfterCommit = true;
    setNeedsCommit();
}
