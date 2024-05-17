void InspectorPageAgent::setShowDebugBorders(ErrorString*, bool show)
{
    m_state->setBoolean(PageAgentState::pageAgentShowDebugBorders, show);
    m_client->setShowDebugBorders(show);
}
