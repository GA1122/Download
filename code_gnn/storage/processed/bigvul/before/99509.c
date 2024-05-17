static NPError NPN_PostURLNotify(NPP npp, const char* url, const char* target, uint32_t len, const char* buf, NPBool file, void* notifyData)
{
    HTTPHeaderMap headerFields;
    Vector<char> postData;
    NPError error = parsePostBuffer(file, buf, len, true, headerFields, postData);
    if (error != NPERR_NO_ERROR)
        return error;

    RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
    plugin->loadURL("POST", makeURLString(url), target, headerFields, postData, true, notifyData);
    return NPERR_NO_ERROR;
}
