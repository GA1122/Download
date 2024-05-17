void InspectorController::resume()
{
    if (InspectorDebuggerAgent* debuggerAgent = m_instrumentingAgents->inspectorDebuggerAgent()) {
        ErrorString error;
        debuggerAgent->resume(&error);
    }
}
