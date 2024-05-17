void InspectorPageAgent::setShowViewportSizeOnResize(ErrorString*, bool show, const bool* showGrid)
{
    m_state->setBoolean(PageAgentState::showSizeOnResize, show);
    m_state->setBoolean(PageAgentState::showGridOnResize, showGrid && *showGrid);
}
