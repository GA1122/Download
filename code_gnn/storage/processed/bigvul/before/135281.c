bool Document::isDelayingLoadEvent()
{
#if ENABLE(OILPAN)
    if (ThreadState::current()->sweepForbidden()) {
        if (!m_loadEventDelayCount)
            checkLoadEventSoon();
        return true;
    }
#endif
    return m_loadEventDelayCount;
}
