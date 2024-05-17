LRESULT WebView::onShowWindowEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    if (!lParam) {
        m_isVisible = wParam;
        if (m_page)
            m_page->viewStateDidChange(WebPageProxy::ViewIsVisible);
    }

    handled = false;
    return 0;
}
