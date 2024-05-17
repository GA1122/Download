void InspectorAgentRegistry::clearFrontend()
{
    for (size_t i = 0; i < m_agents.size(); i++)
        m_agents[i]->clearFrontend();
}
