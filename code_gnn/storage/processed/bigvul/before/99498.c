static void NPN_InvalidateRegion(NPP npp, NPRegion invalidRegion)
{
    RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
    plugin->invalidate(0);
}
