void WebView::setViewNeedsDisplay(const WebCore::IntRect& rect)
{
    RECT r = rect;
    ::InvalidateRect(m_window, &r, false);
}
