void Document::unregisterVisibilityObserver(DocumentVisibilityObserver* observer)
{
    ASSERT(m_visibilityObservers.contains(observer));
    m_visibilityObservers.remove(observer);
}
