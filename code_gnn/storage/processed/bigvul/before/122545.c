void WebDevToolsAgentImpl::didBeginFrame(int frameId)
{
    if (InspectorController* ic = inspectorController())
        ic->didBeginFrame(frameId);
}
