void InspectorClientImpl::setTouchEventEmulationEnabled(bool enabled)
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->setTouchEventEmulationEnabled(enabled);
}
