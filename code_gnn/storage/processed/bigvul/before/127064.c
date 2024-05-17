IntPoint ChromeClientImpl::screenToRootView(const IntPoint& point) const
{
    IntPoint windowPoint(point);

    if (m_webView->client()) {
        WebRect windowRect = m_webView->client()->windowRect();
        windowPoint.move(-windowRect.x, -windowRect.y);
    }

    return windowPoint;
}
