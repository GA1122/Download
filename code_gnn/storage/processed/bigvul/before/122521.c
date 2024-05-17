void InspectorClientImpl::flush()
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->flush();
}
