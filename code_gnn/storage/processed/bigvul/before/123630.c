void InspectorPageAgent::setShowFPSCounter(ErrorString* errorString, bool show)
{
    m_state->setBoolean(PageAgentState::pageAgentShowFPSCounter, show);
    if (show && !forceCompositingMode(errorString))
        return;
    m_client->setShowFPSCounter(show && !m_deviceMetricsOverridden);
}
