bool WebView::onIMEComposition(LPARAM lparam)
{
    LOG(TextInput, "onIMEComposition %s", imeCompositionArgumentNames(lparam).latin1().data());
    HIMC hInputContext = getIMMContext();
    if (!hInputContext)
        return true;

    if (!m_page->selectionState().isContentEditable)
        return true;

    prepareCandidateWindow(hInputContext);

    if (lparam & GCS_RESULTSTR || !lparam) {
        String compositionString;
        if (!getCompositionString(hInputContext, GCS_RESULTSTR, compositionString) && lparam)
            return true;
        
        m_page->confirmComposition(compositionString);
        return true;
    }

    String compositionString;
    if (!getCompositionString(hInputContext, GCS_COMPSTR, compositionString))
        return true;
    
    int numAttributes = Ime::ImmGetCompositionStringW(hInputContext, GCS_COMPATTR, 0, 0);
    Vector<BYTE> attributes(numAttributes);
    Ime::ImmGetCompositionStringW(hInputContext, GCS_COMPATTR, attributes.data(), numAttributes);

    int numBytes = Ime::ImmGetCompositionStringW(hInputContext, GCS_COMPCLAUSE, 0, 0);
    Vector<DWORD> clauses(numBytes / sizeof(DWORD));
    Ime::ImmGetCompositionStringW(hInputContext, GCS_COMPCLAUSE, clauses.data(), numBytes);

    Vector<CompositionUnderline> underlines;
    compositionToUnderlines(clauses, attributes, underlines);

    int cursorPosition = LOWORD(Ime::ImmGetCompositionStringW(hInputContext, GCS_CURSORPOS, 0, 0));

    m_page->setComposition(compositionString, underlines, cursorPosition);

    return true;
}
