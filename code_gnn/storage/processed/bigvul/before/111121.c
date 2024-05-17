void WebPagePrivate::didPluginExitFullScreen(PluginView* plugin, const char* windowUniquePrefix)
{
    m_fullScreenPluginView = 0;
    m_client->didPluginExitFullScreen();

    if (!m_client->window())
        return;

    Platform::Graphics::Window::setTransparencyDiscardFilter(0);
    m_client->window()->setSensitivityFullscreenOverride(false);
}
