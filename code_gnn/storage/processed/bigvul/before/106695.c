HCURSOR WebView::cursorToShow() const
{
    if (!m_page->isValid())
        return 0;

    static HCURSOR arrowCursor = ::LoadCursor(0, IDC_ARROW);
    if (m_overrideCursor && m_webCoreCursor == arrowCursor)
        return m_overrideCursor;

    return m_webCoreCursor;
}
