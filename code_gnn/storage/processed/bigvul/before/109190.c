void InspectorController::didProcessTask()
{
    if (InspectorTimelineAgent* timelineAgent = m_instrumentingAgents->inspectorTimelineAgent())
        timelineAgent->didProcessTask();
    if (InspectorProfilerAgent* profilerAgent = m_instrumentingAgents->inspectorProfilerAgent())
        profilerAgent->didProcessTask();
    if (InspectorDOMDebuggerAgent* domDebuggerAgent = m_instrumentingAgents->inspectorDOMDebuggerAgent())
        domDebuggerAgent->didProcessTask();
}
