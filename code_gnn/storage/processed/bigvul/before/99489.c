static NPError NPN_GetURL(NPP npp, const char* url, const char* target)
{
    if (!url)
        return NPERR_GENERIC_ERROR;
    
    RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
    plugin->loadURL("GET", makeURLString(url), target, HTTPHeaderMap(), Vector<char>(), false, 0);
    
    return NPERR_GENERIC_ERROR;
}
