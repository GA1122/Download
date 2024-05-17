void WebView::prepareCandidateWindow(HIMC hInputContext) 
{
    IntRect caret = m_page->firstRectForCharacterInSelectedRange(0);
    CANDIDATEFORM form;
    form.dwIndex = 0;
    form.dwStyle = CFS_EXCLUDE;
    form.ptCurrentPos.x = caret.x();
    form.ptCurrentPos.y = caret.maxY();
    form.rcArea.top = caret.y();
    form.rcArea.bottom = caret.maxY();
    form.rcArea.left = caret.x();
    form.rcArea.right = caret.maxX();
    Ime::ImmSetCandidateWindow(hInputContext, &form);
}
