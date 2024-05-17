void ChromeClientImpl::focus()
{
    if (m_webView->client())
        m_webView->client()->didFocus();
}
