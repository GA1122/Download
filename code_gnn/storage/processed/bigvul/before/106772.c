void WebView::updateActiveState()
{
    m_page->viewStateDidChange(WebPageProxy::ViewWindowIsActive);
}
