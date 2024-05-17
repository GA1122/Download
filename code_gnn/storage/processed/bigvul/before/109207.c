void InspectorController::reuseFrontend(InspectorFrontendChannel* frontendChannel, const String& inspectorStateCookie)
{
    ASSERT(!m_inspectorFrontend);
    connectFrontend(frontendChannel);
    m_state->loadFromCookie(inspectorStateCookie);
    m_agents.restore();
}
