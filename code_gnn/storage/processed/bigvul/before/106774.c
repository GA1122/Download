void WebView::updateNativeCursor()
{
    m_lastCursorSet = cursorToShow();
    if (!m_lastCursorSet)
        return;
    ::SetCursor(m_lastCursorSet);
}
