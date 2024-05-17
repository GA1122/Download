void DocumentVisibilityObserver::registerObserver(Document& document)
{
    ASSERT(!m_document);
    m_document = &document;
    if (m_document)
        m_document->registerVisibilityObserver(this);
}
