LRESULT WebView::onSizeEvent(HWND, UINT, WPARAM, LPARAM lParam, bool& handled)
{
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);

    if (m_page && m_page->drawingArea()) {
        m_page->drawingArea()->setSize(IntSize(width, height), m_nextResizeScrollOffset);
        m_nextResizeScrollOffset = IntSize();
    }

    handled = true;
    return 0;
}
