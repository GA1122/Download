bool WebPagePrivate::shouldPluginEnterFullScreen(PluginView* plugin, const char* windowUniquePrefix)
{
    return m_client->shouldPluginEnterFullScreen();
}
