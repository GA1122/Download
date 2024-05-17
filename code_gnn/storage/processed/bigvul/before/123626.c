void InspectorPageAgent::setContinuousPaintingEnabled(ErrorString* errorString, bool enabled)
{
    m_state->setBoolean(PageAgentState::pageAgentContinuousPaintingEnabled, enabled);
    if (enabled && !forceCompositingMode(errorString))
        return;
    m_client->setContinuousPaintingEnabled(enabled && !m_deviceMetricsOverridden);
}
