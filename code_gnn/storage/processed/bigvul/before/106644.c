void WebPageProxy::setWindowResizerSize(const IntSize& windowResizerSize)
{
    if (!isValid())
        return;
    process()->send(Messages::WebPage::SetWindowResizerSize(windowResizerSize), m_pageID);
}
