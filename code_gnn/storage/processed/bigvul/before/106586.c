double WebPageProxy::pageZoomFactor() const
{
    return m_mainFrameHasCustomRepresentation ? m_pageClient->customRepresentationZoomFactor() : m_pageZoomFactor;
}
