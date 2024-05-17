void WebView::setIsInWindow(bool isInWindow)
{
    m_isInWindow = isInWindow;
    m_page->viewStateDidChange(WebPageProxy::ViewIsInWindow);
}
