void InspectorController::processGPUEvent(double timestamp, int phase, bool foreign, size_t usedGPUMemoryBytes)
{
    if (InspectorTimelineAgent* timelineAgent = m_instrumentingAgents->inspectorTimelineAgent())
        timelineAgent->processGPUEvent(InspectorTimelineAgent::GPUEvent(timestamp, phase, foreign, usedGPUMemoryBytes));
}
