PepperPluginInfo* PluginServiceImpl::GetRegisteredPpapiPluginInfo(
    const FilePath& plugin_path) {
  PepperPluginInfo* info = NULL;
  for (size_t i = 0; i < ppapi_plugins_.size(); i++) {
    if (ppapi_plugins_[i].path == plugin_path) {
      info = &ppapi_plugins_[i];
      break;
    }
  }
  if (info)
    return info;
  webkit::WebPluginInfo webplugin_info;
  if (!GetPluginInfoByPath(plugin_path, &webplugin_info))
    return NULL;
  PepperPluginInfo new_pepper_info;
  if (!MakePepperPluginInfo(webplugin_info, &new_pepper_info))
    return NULL;
  ppapi_plugins_.push_back(new_pepper_info);
  return &ppapi_plugins_[ppapi_plugins_.size() - 1];
}
