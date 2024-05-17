void InspectorController::evaluateForTestInFrontend(long callId, const String& script)
{
    m_isUnderTest = true;
    if (InspectorInspectorAgent* inspectorAgent = m_instrumentingAgents->inspectorInspectorAgent())
        inspectorAgent->evaluateForTestInFrontend(callId, script);
}
