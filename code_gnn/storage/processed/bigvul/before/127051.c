void NavigatorContentUtilsClientImpl::registerProtocolHandler(const String& scheme, const WebCore::KURL& baseURL, const WebCore::KURL& url, const String& title)
{
    m_webView->client()->registerProtocolHandler(scheme, baseURL, url, title);
}
