::ppapi::FunctionGroupBase* ResourceTracker::GetFunctionAPI(
    PP_Instance pp_instance,
    pp::proxy::InterfaceID id) {
  PluginInstance* instance = GetInstance(pp_instance);
  if (!instance)
    return NULL;

  if (id == pp::proxy::INTERFACE_ID_PPB_INSTANCE)
    return instance;

  scoped_ptr< ::ppapi::FunctionGroupBase >& proxy =
      instance_map_[pp_instance]->function_proxies[id];
  if (proxy.get())
    return proxy.get();

  switch (id) {
    case pp::proxy::INTERFACE_ID_PPB_CHAR_SET:
      proxy.reset(new PPB_CharSet_Impl(instance));
      break;
    case pp::proxy::INTERFACE_ID_PPB_CURSORCONTROL:
      proxy.reset(new PPB_CursorControl_Impl(instance));
      break;
    case pp::proxy::INTERFACE_ID_PPB_FIND:
      proxy.reset(new PPB_Find_Impl(instance));
      break;
    case pp::proxy::INTERFACE_ID_PPB_FONT:
      proxy.reset(new PPB_Font_FunctionImpl(instance));
      break;
    case pp::proxy::INTERFACE_ID_RESOURCE_CREATION:
      proxy.reset(new ResourceCreationImpl(instance));
      break;
    default:
      NOTREACHED();
  }

  return proxy.get();
}
