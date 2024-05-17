static NPError NPN_PostURL(NPP npp, const char* url, const char* target, uint32_t len, const char* buf, NPBool file)
{
    HTTPHeaderMap headerFields;
    Vector<char> postData;
    
    bool parseHeaders = file;

    NPError error = parsePostBuffer(file, buf, len, parseHeaders, headerFields, postData);
    if (error != NPERR_NO_ERROR)
        return error;

    RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
    plugin->loadURL("POST", makeURLString(url), target, headerFields, postData, false, 0);
    return NPERR_NO_ERROR;
}
