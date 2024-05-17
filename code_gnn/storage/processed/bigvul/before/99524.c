static const char* NPN_UserAgent(NPP npp)
{
    if (!npp)
        return 0;
    
    RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
    return plugin->userAgent();
}
