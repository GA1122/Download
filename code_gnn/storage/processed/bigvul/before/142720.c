bool FrameLoader::shouldTreatURLAsSameAsCurrent(const KURL& url) const
{
    return m_currentItem && url == m_currentItem->url();
}
