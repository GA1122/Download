 bool InspectorPageAgent::overrideTextAutosizing(bool textAutosizing)
 {
     if (!m_deviceMetricsOverridden)
        return textAutosizing;
    return m_state->getBoolean(PageAgentState::pageAgentTextAutosizingOverride);
}
