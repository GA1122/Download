void ChromeClientImpl::willSetInputMethodState()
{
    if (m_webView->client())
        m_webView->client()->resetInputMethod();
}
