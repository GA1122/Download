void WebDevToolsAgentImpl::enableViewportEmulation()
{
    if (m_emulateViewportEnabled)
        return;
    m_emulateViewportEnabled = true;
    m_isOverlayScrollbarsEnabled = RuntimeEnabledFeatures::overlayScrollbarsEnabled();
    RuntimeEnabledFeatures::setOverlayScrollbarsEnabled(true);
    m_originalViewportEnabled = RuntimeEnabledFeatures::cssViewportEnabled();
    RuntimeEnabledFeatures::setCSSViewportEnabled(true);
    m_webViewImpl->settings()->setViewportEnabled(true);
    m_webViewImpl->settings()->setViewportMetaEnabled(true);
    m_webViewImpl->settings()->setShrinksViewportContentToFit(true);
    m_webViewImpl->setIgnoreViewportTagScaleLimits(true);
    m_webViewImpl->setPageScaleFactorLimits(-1, -1);
    m_webViewImpl->setZoomFactorOverride(1);
    if (m_touchEventEmulationEnabled)
        m_client->setTouchEventEmulationEnabled(m_touchEventEmulationEnabled, m_emulateViewportEnabled);
}
