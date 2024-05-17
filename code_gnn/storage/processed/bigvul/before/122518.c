void InspectorClientImpl::dispatchKeyEvent(const PlatformKeyboardEvent& event)
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->dispatchKeyEvent(event);
}
