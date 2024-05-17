void InspectorPageAgent::setShowDebugBorders(ErrorString* errorString, bool show)
{
    m_state->setBoolean(PageAgentState::pageAgentShowDebugBorders, show);
    if (show && !forceCompositingMode(errorString))
        return;
    m_client->setShowDebugBorders(show);
}
