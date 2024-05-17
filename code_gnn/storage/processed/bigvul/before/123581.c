void InspectorAgent::appended(InstrumentingAgents* instrumentingAgents, InspectorState* inspectorState)
{
    m_instrumentingAgents = instrumentingAgents;
    m_state = inspectorState;
    init();
}
