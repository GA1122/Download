void PluginServiceImpl::GetInternalPlugins(
    std::vector<webkit::WebPluginInfo>* plugins) {
  plugin_list_->GetInternalPlugins(plugins);
}
