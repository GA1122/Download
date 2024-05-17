void InspectorController::setInjectedScriptForOrigin(const String& origin, const String& source)
{
    if (InspectorInspectorAgent* inspectorAgent = m_instrumentingAgents->inspectorInspectorAgent())
        inspectorAgent->setInjectedScriptForOrigin(origin, source);
}
