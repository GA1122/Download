static NPError NPN_GetURLNotify(NPP npp, const char* url, const char* target, void* notifyData)
{
    if (!url)
        return NPERR_GENERIC_ERROR;

    RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
    plugin->loadURL("GET", makeURLString(url), target, HTTPHeaderMap(), Vector<char>(), true, notifyData);
    
    return NPERR_NO_ERROR;
}
