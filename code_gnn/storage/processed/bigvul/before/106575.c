void WebPageProxy::linkClicked(const String& url, const WebMouseEvent& event)
{
    process()->send(Messages::WebPage::LinkClicked(url, event), m_pageID, 0);
}
