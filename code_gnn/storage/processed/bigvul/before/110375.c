const void* PluginModule::GetPluginInterface(const char* name) const {
  if (out_of_process_proxy_.get())
    return out_of_process_proxy_->GetProxiedInterface(name);

  if (!entry_points_.get_interface)
    return NULL;
  return entry_points_.get_interface(name);
}
