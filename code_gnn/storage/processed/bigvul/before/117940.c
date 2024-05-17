void ScriptController::clearScriptObjects()
{
    PluginObjectMap::iterator it = m_pluginObjects.begin();
    for (; it != m_pluginObjects.end(); ++it) {
        _NPN_UnregisterObject(it->second);
        _NPN_ReleaseObject(it->second);
    }
    m_pluginObjects.clear();

#if ENABLE(NETSCAPE_PLUGIN_API)
    if (m_wrappedWindowScriptNPObject) {
        NPObjectWrapper* windowScriptObjectWrapper = NPObjectWrapper::getWrapper(m_wrappedWindowScriptNPObject);
        ASSERT(windowScriptObjectWrapper);

        NPObject* windowScriptNPObject = NPObjectWrapper::getUnderlyingNPObject(m_wrappedWindowScriptNPObject);
        ASSERT(windowScriptNPObject);
        _NPN_DeallocateObject(windowScriptNPObject);

        windowScriptObjectWrapper->clear();
        _NPN_ReleaseObject(m_wrappedWindowScriptNPObject);
        m_wrappedWindowScriptNPObject = 0;
    }
#endif
}
