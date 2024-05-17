void InspectorAgentRegistry::setFrontend(InspectorFrontend* frontend)
{
    for (size_t i = 0; i < m_agents.size(); i++)
        m_agents[i]->setFrontend(frontend);
}
