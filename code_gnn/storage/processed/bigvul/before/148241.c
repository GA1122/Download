bool BrowserPpapiHostImpl::IsPotentiallySecurePluginContext(
    PP_Instance instance) {
  auto it = instance_map_.find(instance);
  if (it == instance_map_.end())
    return false;
  return it->second->renderer_data.is_potentially_secure_plugin_context;
}
