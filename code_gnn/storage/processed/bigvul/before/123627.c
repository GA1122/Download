void InspectorPageAgent::setDeviceMetricsOverride(ErrorString* errorString, int width, int height, double deviceScaleFactor, bool emulateViewport, bool fitWindow, const bool* optionalTextAutosizing, const double* optionalFontScaleFactor)
{
    const static long maxDimension = 10000000;

    bool textAutosizing = optionalTextAutosizing ? *optionalTextAutosizing : false;
    double fontScaleFactor = optionalFontScaleFactor ? *optionalFontScaleFactor : 1;

    if (width < 0 || height < 0 || width > maxDimension || height > maxDimension) {
        *errorString = "Width and height values must be positive, not greater than " + String::number(maxDimension);
        return;
    }

    if (!width ^ !height) {
        *errorString = "Both width and height must be either zero or non-zero at once";
        return;
    }

    if (deviceScaleFactor <= 0) {
        *errorString = "deviceScaleFactor must be positive";
        return;
    }

    if (fontScaleFactor <= 0) {
        *errorString = "fontScaleFactor must be positive";
        return;
    }

    Settings& settings = m_page->settings();
    if (width && height && !settings.acceleratedCompositingEnabled()) {
        if (errorString)
            *errorString = "Compositing mode is not supported";
        return;
    }

    if (!deviceMetricsChanged(width, height, deviceScaleFactor, emulateViewport, fitWindow, fontScaleFactor, textAutosizing))
        return;


    m_state->setLong(PageAgentState::pageAgentScreenWidthOverride, width);
    m_state->setLong(PageAgentState::pageAgentScreenHeightOverride, height);
    m_state->setDouble(PageAgentState::pageAgentDeviceScaleFactorOverride, deviceScaleFactor);
    m_state->setBoolean(PageAgentState::pageAgentEmulateViewport, emulateViewport);
    m_state->setBoolean(PageAgentState::pageAgentFitWindow, fitWindow);
    m_state->setDouble(PageAgentState::fontScaleFactor, fontScaleFactor);
    m_state->setBoolean(PageAgentState::pageAgentTextAutosizingOverride, textAutosizing);

    updateViewMetrics(width, height, deviceScaleFactor, emulateViewport, fitWindow);
}
