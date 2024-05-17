void PluginServiceImpl::RegisterPepperPlugins() {
  PepperPluginRegistry::ComputeList(&ppapi_plugins_);
  for (size_t i = 0; i < ppapi_plugins_.size(); ++i) {
    RegisterInternalPlugin(ppapi_plugins_[i].ToWebPluginInfo(), true);
  }
}
