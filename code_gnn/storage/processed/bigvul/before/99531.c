NPNetscapeFuncs* netscapeBrowserFuncs()
{
    static NPNetscapeFuncs netscapeFuncs;
    static bool initialized = false;
    
    if (!initialized) {
        initializeBrowserFuncs(netscapeFuncs);
        initialized = true;
    }

    return &netscapeFuncs;
}
