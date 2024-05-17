void PluginChannel::CleanUp() {
  for (size_t i = 0; i < plugin_stubs_.size(); ++i)
    RemoveRoute(plugin_stubs_[i]->instance_id());

  scoped_refptr<PluginChannel> me(this);

  plugin_stubs_.clear();
}
