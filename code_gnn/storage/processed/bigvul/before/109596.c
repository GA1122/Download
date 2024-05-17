WeakPtr<Document> Document::contextDocument()
{
    if (m_contextDocument)
        return m_contextDocument;
    if (m_frame)
        return m_weakFactory.createWeakPtr();
    return WeakPtr<Document>(0);
}
