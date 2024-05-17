void InspectorClientImpl::resetTraceEventCallback()
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->resetTraceEventCallback();
}
