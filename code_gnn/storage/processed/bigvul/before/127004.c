bool ChromeClientImpl::canRunModal()
{
    return !!m_webView->client();
}
