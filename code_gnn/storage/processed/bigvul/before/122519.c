void InspectorClientImpl::dispatchMouseEvent(const PlatformMouseEvent& event)
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->dispatchMouseEvent(event);
}
