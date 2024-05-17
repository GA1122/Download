void InspectorController::processGPUEvent(double timestamp, int phase, bool foreign, uint64_t usedGPUMemoryBytes, uint64_t limitGPUMemoryBytes)
{
    if (InspectorTimelineAgent* timelineAgent = m_instrumentingAgents->inspectorTimelineAgent())
        timelineAgent->processGPUEvent(InspectorTimelineAgent::GPUEvent(timestamp, phase, foreign, usedGPUMemoryBytes, limitGPUMemoryBytes));
}
