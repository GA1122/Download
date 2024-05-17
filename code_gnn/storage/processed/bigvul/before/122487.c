void InspectorController::willBeDestroyed()
{
    disconnectFrontend();
    m_injectedScriptManager->disconnect();
    m_inspectorClient = 0;
    m_page = 0;
    m_instrumentingAgents->reset();
    m_agents.discardAgents();
}
