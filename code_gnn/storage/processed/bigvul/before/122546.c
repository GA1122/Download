void WebDevToolsAgentImpl::didCancelFrame()
{
    if (InspectorController* ic = inspectorController())
        ic->didCancelFrame();
}
