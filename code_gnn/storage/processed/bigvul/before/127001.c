void ChromeClientImpl::annotatedRegionsChanged()
{
    WebViewClient* client = m_webView->client();
    if (client)
        client->draggableRegionsChanged();
}
