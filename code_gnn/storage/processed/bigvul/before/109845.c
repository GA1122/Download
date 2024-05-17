void Document::styleResolverThrowawayTimerFired(Timer<Document>*)
{
    if (m_styleResolverAccessCount == m_lastStyleResolverAccessCount)
        clearStyleResolver();
    m_lastStyleResolverAccessCount = m_styleResolverAccessCount;
}
