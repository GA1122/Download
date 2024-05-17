void WebView::flashBackingStoreUpdates(const Vector<IntRect>& updateRects)
{
    static HBRUSH brush = createBrush(WebPageProxy::backingStoreUpdatesFlashColor().rgb()).leakPtr();
    HDC dc = ::GetDC(m_window);
    flashRects(dc, updateRects.data(), updateRects.size(), brush);
    ::ReleaseDC(m_window, dc);
}
