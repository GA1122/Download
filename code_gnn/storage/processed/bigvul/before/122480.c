void InspectorController::registerModuleAgent(PassOwnPtr<InspectorAgent> agent)
{
    m_moduleAgents.append(agent.get());
    m_agents.append(agent);
}
