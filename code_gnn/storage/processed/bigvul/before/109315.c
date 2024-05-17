bool InspectorPageAgent::overrideTextAutosizing(bool textAutosizing)
{
    if (m_enabled)
        return m_state->getBoolean(PageAgentState::pageAgentTextAutosizingOverride);
    return textAutosizing;
}
