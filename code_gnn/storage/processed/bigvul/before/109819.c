void Document::setParsing(bool b)
{
    m_bParsing = b;

    if (m_bParsing && !m_sharedObjectPool)
        m_sharedObjectPool = DocumentSharedObjectPool::create();

    if (!m_bParsing && view())
        view()->scheduleRelayout();
}
