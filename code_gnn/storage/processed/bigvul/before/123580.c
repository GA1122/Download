 void InspectorAgentRegistry::append(PassOwnPtr<InspectorAgent> agent)
 {
     agent->appended(m_instrumentingAgents, m_inspectorState->createAgentState(agent->name()));
    m_agents.append(agent);
}
