static NPError NPN_GetValue(NPP npp, NPNVariable variable, void *value)
{
    switch (variable) {
        case NPNVWindowNPObject: {
            RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);

            NPObject* windowNPObject = plugin->windowScriptNPObject();
            *(NPObject**)value = windowNPObject;
            break;
        }
        case NPNVPluginElementNPObject: {
            RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);

            NPObject* pluginElementNPObject = plugin->pluginElementNPObject();
            *(NPObject**)value = pluginElementNPObject;
            break;
        }
#if PLATFORM(MAC)
        case NPNVsupportsCoreGraphicsBool:
            *(NPBool*)value = true;
            break;

        case NPNVsupportsCoreAnimationBool:
            *(NPBool*)value = false;
            break;

        case NPNVsupportsCocoaBool:
            *(NPBool*)value = true;
            break;

#ifndef NP_NO_CARBON
       case NPNVsupportsCarbonBool:
            *(NPBool*)value = false;
            break;
#endif

#endif
        default:
            notImplemented();
            return NPERR_GENERIC_ERROR;
    }

    return NPERR_NO_ERROR;
}
