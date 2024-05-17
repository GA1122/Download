void WebDevToolsAgentImpl::webViewResized(const WebSize& size)
{
    if (InspectorController* ic = inspectorController())
        ic->webViewResized(IntSize());
}
