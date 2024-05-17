void NavigatorContentUtilsClientImpl::unregisterProtocolHandler(const String& scheme, const WebCore::KURL& baseURL, const WebCore::KURL& url)
{
    m_webView->client()->unregisterProtocolHandler(scheme, baseURL, url);
}
