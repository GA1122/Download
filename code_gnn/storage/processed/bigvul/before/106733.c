bool WebView::onIMEStartComposition()
{
    LOG(TextInput, "onIMEStartComposition");
    m_inIMEComposition++;

    HIMC hInputContext = getIMMContext();
    if (!hInputContext)
        return false;
    prepareCandidateWindow(hInputContext);
    Ime::ImmReleaseContext(m_window, hInputContext);
    return true;
}
