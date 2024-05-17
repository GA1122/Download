void Range::setDocument(Document* document)
{
    ASSERT(m_ownerDocument != document);
    if (m_ownerDocument)
        m_ownerDocument->detachRange(this);
    m_ownerDocument = document;
    m_start.setToStartOfNode(document);
    m_end.setToStartOfNode(document);
    m_ownerDocument->attachRange(this);
}
