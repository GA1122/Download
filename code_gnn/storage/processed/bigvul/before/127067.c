void ChromeClientImpl::setCursor(const WebCursorInfo& cursor)
{
#if OS(MACOSX)
    if (m_webView->hasOpenedPopup())
        return;
#endif
    if (m_webView->client())
        m_webView->client()->didChangeCursor(cursor);
}
