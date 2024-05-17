bool InspectorPageAgent::deviceMetricsChanged(int width, int height, double deviceScaleFactor, bool fitWindow, bool textAutosizing)
{
    int currentWidth = static_cast<int>(m_state->getLong(PageAgentState::pageAgentScreenWidthOverride));
    int currentHeight = static_cast<int>(m_state->getLong(PageAgentState::pageAgentScreenHeightOverride));
    double currentDeviceScaleFactor = m_state->getDouble(PageAgentState::pageAgentDeviceScaleFactorOverride, 1);
    bool currentFitWindow = m_state->getBoolean(PageAgentState::pageAgentFitWindow);
    bool currentTextAutosizing = m_state->getBoolean(PageAgentState::pageAgentTextAutosizingOverride);

    return width != currentWidth || height != currentHeight || deviceScaleFactor != currentDeviceScaleFactor || fitWindow != currentFitWindow || textAutosizing != currentTextAutosizing;
}
