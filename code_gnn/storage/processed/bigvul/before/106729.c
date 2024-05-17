LRESULT WebView::onIMERequestCharPosition(IMECHARPOSITION* charPos)
{
    if (charPos->dwCharPos && !m_page->selectionState().hasComposition)
        return 0;
    IntRect caret = m_page->firstRectForCharacterInSelectedRange(charPos->dwCharPos);
    charPos->pt.x = caret.x();
    charPos->pt.y = caret.y();
    ::ClientToScreen(m_window, &charPos->pt);
    charPos->cLineHeight = caret.height();
    ::GetWindowRect(m_window, &charPos->rcDocument);
    return true;
}
