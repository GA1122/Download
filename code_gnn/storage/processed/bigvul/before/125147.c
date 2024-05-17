void PluginServiceImpl::RegisterInternalPlugin(
    const webkit::WebPluginInfo& info,
    bool add_at_beginning) {
  plugin_list_->RegisterInternalPlugin(info, add_at_beginning);
}
