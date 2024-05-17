InspectorController::~InspectorController()
{
    m_instrumentingAgents->reset();
    m_agents.discardAgents();
    ASSERT(!m_inspectorClient);
}
