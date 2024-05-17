void WebDevToolsAgentImpl::overrideDeviceMetrics(int width, int height, float deviceScaleFactor, bool emulateViewport, bool fitWindow)
{
    if (!width && !height && !deviceScaleFactor) {
        if (m_deviceMetricsEnabled) {
            m_deviceMetricsEnabled = false;
            m_webViewImpl->setBackgroundColorOverride(Color::transparent);
            disableViewportEmulation();
            m_client->disableDeviceEmulation();
        }
    } else {
        if (!m_deviceMetricsEnabled) {
            m_deviceMetricsEnabled = true;
            m_webViewImpl->setBackgroundColorOverride(Color::darkGray);
        }
        if (emulateViewport)
            enableViewportEmulation();
        else
            disableViewportEmulation();

        WebDeviceEmulationParams params;
        params.screenPosition = emulateViewport ? WebDeviceEmulationParams::Mobile : WebDeviceEmulationParams::Desktop;
        params.deviceScaleFactor = deviceScaleFactor;
        params.viewSize = WebSize(width, height);
        params.fitToView = fitWindow;
        params.viewInsets = WebSize(10, 10);
        m_client->enableDeviceEmulation(params);
    }
}
