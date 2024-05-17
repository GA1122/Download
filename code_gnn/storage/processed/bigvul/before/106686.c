void WebView::close()
{
    m_undoClient.initialize(0);
    ::RevokeDragDrop(m_window);
    setParentWindow(0);
    m_page->close();
}
