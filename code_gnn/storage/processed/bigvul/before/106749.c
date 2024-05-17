void WebView::processDidCrash()
{
    updateNativeCursor();
    ::InvalidateRect(m_window, 0, TRUE);
}
