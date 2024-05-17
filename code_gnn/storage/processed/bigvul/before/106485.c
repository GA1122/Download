void WebPageProxy::didFindZoomableArea(const WebCore::IntRect& area)
{
    m_pageClient->didFindZoomableArea(area);
}
