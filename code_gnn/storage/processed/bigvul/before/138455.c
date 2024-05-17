void DocumentVisibilityObserver::unregisterObserver()
{
    if (m_document) {
        m_document->unregisterVisibilityObserver(this);
        m_document = nullptr;
    }
}
