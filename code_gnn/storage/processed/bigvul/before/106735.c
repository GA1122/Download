LRESULT WebView::onKillFocusEvent(HWND, UINT, WPARAM, LPARAM lParam, bool& handled)
{
    m_page->viewStateDidChange(WebPageProxy::ViewIsFocused);
    handled = true;
    return 0;
}