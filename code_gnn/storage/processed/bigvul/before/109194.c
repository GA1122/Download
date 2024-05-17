void InspectorController::evaluateForTestInFrontend(long callId, const String& script)
{
    m_isUnderTest = true;
    if (InspectorAgent* inspectorAgent = m_instrumentingAgents->inspectorAgent())
        inspectorAgent->evaluateForTestInFrontend(callId, script);
}
