void InspectorClientImpl::startGPUEventsRecording()
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->startGPUEventsRecording();
}
