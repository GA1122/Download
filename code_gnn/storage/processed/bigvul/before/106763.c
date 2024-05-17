void WebView::setParentWindow(HWND parentWindow)
{
    if (m_window) {
        if (::GetParent(m_window) == parentWindow)
            return;
        if (parentWindow)
            ::SetParent(m_window, parentWindow);
        else if (!m_isBeingDestroyed) {
            ::SetParent(m_window, HWND_MESSAGE);
        }
    }

    windowAncestryDidChange();
}
