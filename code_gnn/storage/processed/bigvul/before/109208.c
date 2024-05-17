void InspectorController::setInjectedScriptForOrigin(const String& origin, const String& source)
{
    if (InspectorAgent* inspectorAgent = m_instrumentingAgents->inspectorAgent())
        inspectorAgent->setInjectedScriptForOrigin(origin, source);
}
