void Document::decrementLoadEventDelayCount()
{
    DCHECK(m_loadEventDelayCount);
    --m_loadEventDelayCount;

    if (!m_loadEventDelayCount)
        checkLoadEventSoon();
}
