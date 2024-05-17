bool PluginModule::InitializeModule(const EntryPoints& entry_points) {
  DCHECK(!out_of_process_proxy_.get()) << "Don't call for proxied modules.";
  DCHECK(entry_points.initialize_module != NULL);
  int retval = entry_points.initialize_module(pp_module(), &GetInterface);
  if (retval != 0) {
    LOG(WARNING) << "PPP_InitializeModule returned failure " << retval;
    return false;
  }
  return true;
}
