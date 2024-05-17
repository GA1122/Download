bool Document::isDelayingLoadEvent()
{
    if (ThreadState::current()->sweepForbidden()) {
        if (!m_loadEventDelayCount)
            checkLoadEventSoon();
        return true;
    }
    return m_loadEventDelayCount;
}
