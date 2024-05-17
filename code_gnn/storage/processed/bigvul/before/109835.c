void Document::sharedObjectPoolClearTimerFired(Timer<Document>*)
{
    m_sharedObjectPool.clear();
}
