void InspectorClientImpl::highlight()
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->highlight();
}
