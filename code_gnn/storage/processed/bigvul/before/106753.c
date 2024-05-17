void WebView::resetIME()
{
    HIMC hInputContext = getIMMContext();
    if (!hInputContext)
        return;
    Ime::ImmNotifyIME(hInputContext, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
    Ime::ImmReleaseContext(m_window, hInputContext);
}
