void WebView::toolTipChanged(const String&, const String& newToolTip)
{
    if (!m_toolTipWindow)
        return;

    if (!newToolTip.isEmpty()) {
        String toolTip = newToolTip;

        TOOLINFO info = {0};
        info.cbSize = sizeof(info);
        info.uFlags = TTF_IDISHWND;
        info.uId = reinterpret_cast<UINT_PTR>(m_window);
        info.lpszText = const_cast<UChar*>(toolTip.charactersWithNullTermination());
        ::SendMessage(m_toolTipWindow, TTM_UPDATETIPTEXT, 0, reinterpret_cast<LPARAM>(&info));
    }

    ::SendMessage(m_toolTipWindow, TTM_ACTIVATE, !newToolTip.isEmpty(), 0);
}
