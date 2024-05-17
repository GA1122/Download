void InspectorPageAgent::setTouchEmulationEnabled(ErrorString*, bool enabled)
{
    if (m_state->getBoolean(PageAgentState::touchEventEmulationEnabled) == enabled)
        return;
    updateTouchEventEmulationInPage(enabled);
}
