void WebView::didRelaunchProcess()
{
    updateNativeCursor();
    ::InvalidateRect(m_window, 0, TRUE);
}
