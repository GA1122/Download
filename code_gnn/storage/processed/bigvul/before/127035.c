NavigatorContentUtilsClient::CustomHandlersState NavigatorContentUtilsClientImpl::isProtocolHandlerRegistered(const String& scheme, const WebCore::KURL& baseURL, const WebCore::KURL& url)
{
    return static_cast<NavigatorContentUtilsClient::CustomHandlersState>(m_webView->client()->isProtocolHandlerRegistered(scheme, baseURL, url));
}
