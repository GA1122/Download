void Document::registerVisibilityObserver(DocumentVisibilityObserver* observer)
{
    ASSERT(!m_visibilityObservers.contains(observer));
    m_visibilityObservers.add(observer);
}
