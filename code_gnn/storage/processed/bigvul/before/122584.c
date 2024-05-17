void WebDevToolsAgentImpl::setTraceEventCallback(const String& categoryFilter, TraceEventCallback callback)
{
    m_client->setTraceEventCallback(categoryFilter, callback);
}
