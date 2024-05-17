void WebDevToolsAgentImpl::processGPUEvent(const GPUEvent& event)
{
    if (InspectorController* ic = inspectorController())
        ic->processGPUEvent(event.timestamp, event.phase, event.foreign, event.usedGPUMemoryBytes, event.limitGPUMemoryBytes);
}
