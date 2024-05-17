void WebPageProxy::handleTouchEvent(const WebTouchEvent& event)
{
    if (!isValid())
        return;
    process()->send(Messages::WebPage::TouchEvent(event), m_pageID); 
}
