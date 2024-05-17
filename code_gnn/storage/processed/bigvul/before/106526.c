void WebPageProxy::findZoomableAreaForPoint(const WebCore::IntPoint& point)
{
    if (!isValid())
        return;

    process()->send(Messages::WebPage::FindZoomableAreaForPoint(point), m_pageID);
}
