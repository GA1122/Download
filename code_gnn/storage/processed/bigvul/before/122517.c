WebDevToolsAgentImpl* InspectorClientImpl::devToolsAgent()
{
    return static_cast<WebDevToolsAgentImpl*>(m_inspectedWebView->devToolsAgent());
}
