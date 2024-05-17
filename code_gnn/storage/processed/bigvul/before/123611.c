void InspectorPageAgent::disable(ErrorString*)
{
    m_enabled = false;
    m_state->setBoolean(PageAgentState::pageAgentEnabled, false);
    m_state->remove(PageAgentState::pageAgentScriptsToEvaluateOnLoad);
    m_overlay->hide();
    m_instrumentingAgents->setInspectorPageAgent(0);
    m_deviceMetricsOverridden = false;

    setShowPaintRects(0, false);
    setShowDebugBorders(0, false);
    setShowFPSCounter(0, false);
    setEmulatedMedia(0, String());
    setContinuousPaintingEnabled(0, false);
    setShowScrollBottleneckRects(0, false);
    setShowViewportSizeOnResize(0, false, 0);

    if (!deviceMetricsChanged(0, 0, 1, false, false, 1, false))
        return;

    updateViewMetrics(0, 0, 1, false, false);
    m_state->setLong(PageAgentState::pageAgentScreenWidthOverride, 0);
    m_state->setLong(PageAgentState::pageAgentScreenHeightOverride, 0);
    m_state->setDouble(PageAgentState::pageAgentDeviceScaleFactorOverride, 1);
    m_state->setBoolean(PageAgentState::pageAgentEmulateViewport, false);
    m_state->setBoolean(PageAgentState::pageAgentFitWindow, false);
    m_state->setDouble(PageAgentState::fontScaleFactor, 1);
    m_state->setBoolean(PageAgentState::pageAgentTextAutosizingOverride, false);
}
