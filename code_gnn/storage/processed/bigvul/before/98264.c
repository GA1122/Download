unsigned WebFrame::pendingUnloadCount() const
{
    if (!m_coreFrame)
        return 0;

    return m_coreFrame->domWindow()->pendingUnloadEventListeners();
}
