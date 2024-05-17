NodeIntersectionObserverData& Document::ensureIntersectionObserverData()
{
    if (!m_intersectionObserverData)
        m_intersectionObserverData = new NodeIntersectionObserverData();
    return *m_intersectionObserverData;
}
