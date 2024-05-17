PluginModule::~PluginModule() {
  CHECK(!is_in_destructor_);
  is_in_destructor_ = true;

  DCHECK(instances_.empty());

  GetLivePluginSet()->erase(this);

  callback_tracker_->AbortAll();

  if (entry_points_.shutdown_module)
    entry_points_.shutdown_module();

  if (library_)
    base::UnloadNativeLibrary(library_);

  HostGlobals::Get()->ModuleDeleted(pp_module_);
  if (!is_crashed_ && lifetime_delegate_) {
    lifetime_delegate_->PluginModuleDead(this);
  }

}
