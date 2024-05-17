PluginInstance* PluginModule::CreateInstance(PluginDelegate* delegate) {
  PluginInstance* instance = PluginInstance::Create(delegate, this);
  if (!instance) {
    LOG(WARNING) << "Plugin doesn't support instance interface, failing.";
    return NULL;
  }
  if (out_of_process_proxy_.get())
    out_of_process_proxy_->AddInstance(instance->pp_instance());
  return instance;
}
