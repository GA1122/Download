void InspectorClientImpl::setTraceEventCallback(const String& categoryFilter, TraceEventCallback callback)
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->setTraceEventCallback(categoryFilter, callback);
}
