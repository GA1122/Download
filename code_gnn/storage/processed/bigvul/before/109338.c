void InspectorPageAgent::webViewResized(const IntSize& size)
{
    int currentWidth = static_cast<int>(m_state->getLong(PageAgentState::pageAgentScreenWidthOverride));
    m_overlay->resize(currentWidth ? size : IntSize());
}
