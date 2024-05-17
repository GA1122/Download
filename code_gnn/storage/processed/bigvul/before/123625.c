void InspectorPageAgent::restore()
{
    if (m_state->getBoolean(PageAgentState::pageAgentEnabled)) {
        ErrorString error;
        enable(&error);
        bool scriptExecutionDisabled = m_state->getBoolean(PageAgentState::pageAgentScriptExecutionDisabled);
        setScriptExecutionDisabled(0, scriptExecutionDisabled);
        bool showPaintRects = m_state->getBoolean(PageAgentState::pageAgentShowPaintRects);
        setShowPaintRects(0, showPaintRects);
        bool showDebugBorders = m_state->getBoolean(PageAgentState::pageAgentShowDebugBorders);
        setShowDebugBorders(0, showDebugBorders);
        bool showFPSCounter = m_state->getBoolean(PageAgentState::pageAgentShowFPSCounter);
        setShowFPSCounter(0, showFPSCounter);
        String emulatedMedia = m_state->getString(PageAgentState::pageAgentEmulatedMedia);
        setEmulatedMedia(0, emulatedMedia);
        bool continuousPaintingEnabled = m_state->getBoolean(PageAgentState::pageAgentContinuousPaintingEnabled);
        setContinuousPaintingEnabled(0, continuousPaintingEnabled);
        bool showScrollBottleneckRects = m_state->getBoolean(PageAgentState::pageAgentShowScrollBottleneckRects);
        setShowScrollBottleneckRects(0, showScrollBottleneckRects);

        int currentWidth = static_cast<int>(m_state->getLong(PageAgentState::pageAgentScreenWidthOverride));
        int currentHeight = static_cast<int>(m_state->getLong(PageAgentState::pageAgentScreenHeightOverride));
        double currentDeviceScaleFactor = m_state->getDouble(PageAgentState::pageAgentDeviceScaleFactorOverride);
        bool currentEmulateViewport = m_state->getBoolean(PageAgentState::pageAgentEmulateViewport);
        bool currentFitWindow = m_state->getBoolean(PageAgentState::pageAgentFitWindow);
        updateViewMetrics(currentWidth, currentHeight, currentDeviceScaleFactor, currentEmulateViewport, currentFitWindow);
        updateTouchEventEmulationInPage(m_state->getBoolean(PageAgentState::touchEventEmulationEnabled));
    }
}
