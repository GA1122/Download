void WebPagePrivate::didPluginEnterFullScreen(PluginView* plugin, const char* windowUniquePrefix)
{
    m_fullScreenPluginView = plugin;
    m_client->didPluginEnterFullScreen();

    if (!m_client->window())
        return;

    Platform::Graphics::Window::setTransparencyDiscardFilter(windowUniquePrefix);
    m_client->window()->setSensitivityFullscreenOverride(true);
}
