WebDevToolsAgentImpl::WebDevToolsAgentImpl(
    WebViewImpl* webViewImpl,
    WebDevToolsAgentClient* client)
    : m_hostId(client->hostIdentifier())
    , m_client(client)
    , m_webViewImpl(webViewImpl)
    , m_attached(false)
    , m_generatingEvent(false)
    , m_deviceMetricsEnabled(false)
    , m_emulateViewportEnabled(false)
    , m_originalViewportEnabled(false)
    , m_isOverlayScrollbarsEnabled(false)
    , m_touchEventEmulationEnabled(false)
{
    ASSERT(m_hostId > 0);
    ClientMessageLoopAdapter::ensureClientMessageLoopCreated(m_client);
}
