NPError NPN_SetValue(NPP id, NPPVariable variable, void* value) {

  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  switch(variable) {
    case NPPVpluginWindowBool: {
      plugin->set_windowless(value == 0);
      return NPERR_NO_ERROR;
    }
    case NPPVpluginTransparentBool: {
      bool mode = (value != 0);
      plugin->set_transparent(mode);
      return NPERR_NO_ERROR;
    }
    case NPPVjavascriptPushCallerBool:
      DLOG(INFO) <<
          "NPN_SetValue(NPPVJavascriptPushCallerBool) is not implemented.";
      return NPERR_GENERIC_ERROR;
    case NPPVpluginKeepLibraryInMemory:
      DLOG(INFO) <<
          "NPN_SetValue(NPPVpluginKeepLibraryInMemory) is not implemented.";
      return NPERR_GENERIC_ERROR;
  #if defined(OS_MACOSX)
    case NPPVpluginDrawingModel: {
      int model = reinterpret_cast<int>(value);
      if (model == NPDrawingModelCoreGraphics) {
        plugin->set_drawing_model(model);
        return NPERR_NO_ERROR;
      }
      return NPERR_GENERIC_ERROR;
    }
    case NPPVpluginEventModel: {
      int model = reinterpret_cast<int>(value);
      switch (model) {
        case NPEventModelCarbon:
        case NPEventModelCocoa:
          plugin->set_event_model(model);
          return NPERR_NO_ERROR;
          break;
      }
      return NPERR_GENERIC_ERROR;
    }
  #endif
    default:
      DLOG(INFO) << "NPN_SetValue(" << variable << ") is not implemented.";
      break;
  }

  NOTREACHED();
  return NPERR_GENERIC_ERROR;
}
