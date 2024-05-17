void WebView::compositionSelectionChanged(bool hasChanged)
{
    if (m_page->selectionState().hasComposition && !hasChanged)
        resetIME();
}
