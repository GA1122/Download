void WebPageProxy::setResizesToContentsUsingLayoutSize(const WebCore::IntSize& targetLayoutSize)
{
    process()->send(Messages::WebPage::SetResizesToContentsUsingLayoutSize(targetLayoutSize), m_pageID);
}
