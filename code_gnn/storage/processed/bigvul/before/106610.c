void WebPageProxy::scaleWebView(double scale, const IntPoint& origin)
{
    if (!isValid())
        return;

    process()->send(Messages::WebPage::ScaleWebView(scale, origin), m_pageID);
}
