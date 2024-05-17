static NPError NPN_DestroyStream(NPP npp, NPStream* stream, NPReason reason)
{
    RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
    
    return plugin->destroyStream(stream, reason);
}
