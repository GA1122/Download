void Document::styleResolverThrowawayTimerFired(Timer<Document>*)
{
    ASSERT(!m_inStyleRecalc);
    clearStyleResolver();
}
