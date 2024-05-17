GURL BrowserPpapiHostImpl::GetPluginURLForInstance(PP_Instance instance) {
  auto it = instance_map_.find(instance);
  if (it == instance_map_.end())
    return GURL();
  return it->second->renderer_data.plugin_url;
}
