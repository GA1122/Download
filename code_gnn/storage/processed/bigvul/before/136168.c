void Document::loadPluginsSoon()
{
    if (!m_pluginLoadingTimer.isActive())
        m_pluginLoadingTimer.startOneShot(0, FROM_HERE);
}
