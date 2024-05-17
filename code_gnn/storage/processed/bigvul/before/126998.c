String ChromeClientImpl::acceptLanguages()
{
    return m_webView->client()->acceptLanguages();
}
