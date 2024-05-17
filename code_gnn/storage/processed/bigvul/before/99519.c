static NPError NPN_SetValue(NPP npp, NPPVariable variable, void *value)
{
    switch (variable) {
#if PLATFORM(MAC)
        case NPPVpluginDrawingModel: {
            RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
            
            NPDrawingModel drawingModel = static_cast<NPDrawingModel>(reinterpret_cast<uintptr_t>(value));
            return plugin->setDrawingModel(drawingModel);
        }

        case NPPVpluginEventModel: {
            RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
            
            NPEventModel eventModel = static_cast<NPEventModel>(reinterpret_cast<uintptr_t>(value));
            return plugin->setEventModel(eventModel);
        }
#endif

        default:
            notImplemented();
            return NPERR_GENERIC_ERROR;
    }
    
    return NPERR_NO_ERROR;
}
