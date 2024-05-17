void WebPageProxy::setActualVisibleContentRect(const IntRect& rect)
{
    if (!isValid())
        return;

    process()->send(Messages::WebPage::SetActualVisibleContentRect(rect), m_pageID);
}
