void PluginModule::InstanceCreated(PluginInstance* instance) {
  instances_.insert(instance);
}
