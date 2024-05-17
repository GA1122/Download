void WebDevToolsAgentImpl::disableViewportEmulation()
{
    if (!m_emulateViewportEnabled)
        return;
    RuntimeEnabledFeatures::setOverlayScrollbarsEnabled(m_isOverlayScrollbarsEnabled);
    RuntimeEnabledFeatures::setCSSViewportEnabled(m_originalViewportEnabled);
    m_webViewImpl->settings()->setViewportEnabled(false);
    m_webViewImpl->settings()->setViewportMetaEnabled(false);
    m_webViewImpl->settings()->setShrinksViewportContentToFit(false);
    m_webViewImpl->setIgnoreViewportTagScaleLimits(false);
    m_webViewImpl->setPageScaleFactorLimits(1, 1);
    m_webViewImpl->setZoomFactorOverride(0);
    m_emulateViewportEnabled = false;
    if (m_touchEventEmulationEnabled)
        m_client->setTouchEventEmulationEnabled(m_touchEventEmulationEnabled, m_emulateViewportEnabled);
}
