float InspectorPageAgent::overrideFontScaleFactor(float fontScaleFactor)
{
    if (!m_deviceMetricsOverridden)
        return fontScaleFactor;
    return static_cast<float>(m_state->getDouble(PageAgentState::fontScaleFactor));
}
