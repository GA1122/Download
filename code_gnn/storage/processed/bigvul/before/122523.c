void InspectorClientImpl::hideHighlight()
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->hideHighlight();
}
