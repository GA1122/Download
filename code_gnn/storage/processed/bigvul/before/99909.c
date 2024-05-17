NPError NPN_GetValue(NPP id, NPNVariable variable, void* value) {

  NPError rv = NPERR_GENERIC_ERROR;

  switch (variable) {
    case NPNVWindowNPObject: {
      scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
      NPObject *np_object = plugin->webplugin()->GetWindowScriptNPObject();
      if (np_object) {
        WebBindings::retainObject(np_object);
        void **v = (void **)value;
        *v = np_object;
        rv = NPERR_NO_ERROR;
      } else {
        NOTREACHED();
      }
      break;
    }
    case NPNVPluginElementNPObject: {
      scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
      NPObject *np_object = plugin->webplugin()->GetPluginElement();
      if (np_object) {
        WebBindings::retainObject(np_object);
        void** v = static_cast<void**>(value);
        *v = np_object;
        rv = NPERR_NO_ERROR;
      } else {
        NOTREACHED();
      }
      break;
    }
  #if !defined(OS_MACOSX)   
    case NPNVnetscapeWindow: {
      scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
      gfx::PluginWindowHandle handle = plugin->window_handle();
      *((void**)value) = (void*)handle;
      rv = NPERR_NO_ERROR;
      break;
    }
  #endif
    case NPNVjavascriptEnabledBool: {
      *((void**)value) = (void*)1;
      rv = NPERR_NO_ERROR;
      break;
    }
  #if defined(TOOLKIT_USES_GTK)
    case NPNVToolkit:
      *reinterpret_cast<int*>(value) = NPNVGtk2;
      rv = NPERR_NO_ERROR;
      break;

    case NPNVSupportsXEmbedBool:
      *reinterpret_cast<NPBool*>(value) = TRUE;
      rv = NPERR_NO_ERROR;
      break;
  #endif
    case NPNVSupportsWindowless: {
      NPBool* supports_windowless = reinterpret_cast<NPBool*>(value);
      *supports_windowless = TRUE;
      rv = NPERR_NO_ERROR;
      break;
    }
    case NPNVprivateModeBool: {
      NPBool* private_mode = reinterpret_cast<NPBool*>(value);
      scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
      *private_mode = plugin->webplugin()->IsOffTheRecord();
      rv = NPERR_NO_ERROR;
      break;
    }
    case default_plugin::kMissingPluginStatusStart +
         default_plugin::MISSING_PLUGIN_AVAILABLE:
    case default_plugin::kMissingPluginStatusStart +
         default_plugin::MISSING_PLUGIN_USER_STARTED_DOWNLOAD: {
      scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
      if (plugin->plugin_lib()->plugin_info().path.value() ==
            kDefaultPluginLibraryName) {
        plugin->webplugin()->OnMissingPluginStatus(
            variable - default_plugin::kMissingPluginStatusStart);
      }
      break;
    }
  #if defined(OS_MACOSX)
    case NPNVpluginDrawingModel: {
      scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
      *reinterpret_cast<int*>(value) = plugin->drawing_model();
      rv = NPERR_NO_ERROR;
      break;
    }
    case NPNVsupportsQuickDrawBool: {
      NPBool* supports_qd = reinterpret_cast<NPBool*>(value);
      *supports_qd = FALSE;
      rv = NPERR_NO_ERROR;
      break;
    }
    case NPNVsupportsCoreGraphicsBool:
    case NPNVsupportsCarbonBool:
    case NPNVsupportsCocoaBool: {
      NPBool* supports_model = reinterpret_cast<NPBool*>(value);
      *supports_model = TRUE;
      rv = NPERR_NO_ERROR;
      break;
    }
    case NPNVsupportsOpenGLBool:
    case NPNVsupportsCoreAnimationBool: {
      NPBool* supports_model = reinterpret_cast<NPBool*>(value);
      *supports_model = FALSE;
      rv = NPERR_NO_ERROR;
      break;
    }
  #endif   
    case NPNVPepperExtensions:
      rv = NPAPI::GetPepperExtensionsFunctions(value);
      break;
    default:
      DLOG(INFO) << "NPN_GetValue(" << variable << ") is not implemented yet.";
      break;
  }
  return rv;
}
