void InspectorPageAgent::updateViewMetrics(int width, int height, double deviceScaleFactor, bool emulateViewport, bool fitWindow)
{
    if (width && height && !m_page->settings().acceleratedCompositingEnabled())
        return;

    m_deviceMetricsOverridden = width && height;
    m_emulateViewportEnabled = emulateViewport;
    m_client->overrideDeviceMetrics(width, height, static_cast<float>(deviceScaleFactor), emulateViewport, fitWindow);

    Document* document = mainFrame()->document();
    if (document) {
        document->styleResolverChanged(RecalcStyleImmediately);
        document->mediaQueryAffectingValueChanged();
    }
    InspectorInstrumentation::mediaQueryResultChanged(document);

    m_client->setShowFPSCounter(m_state->getBoolean(PageAgentState::pageAgentShowFPSCounter) && !m_deviceMetricsOverridden);
    m_client->setContinuousPaintingEnabled(m_state->getBoolean(PageAgentState::pageAgentContinuousPaintingEnabled) && !m_deviceMetricsOverridden);
}
