void ChromeClientImpl::setScrollbarsVisible(bool value)
{
    m_scrollbarsVisible = value;
    WebFrameImpl* webFrame = toWebFrameImpl(m_webView->mainFrame());
    if (webFrame)
        webFrame->setCanHaveScrollbars(value);
}
