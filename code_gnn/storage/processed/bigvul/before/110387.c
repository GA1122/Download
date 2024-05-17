void PluginModule::PluginCrashed() {
  DCHECK(!is_crashed_);   
  is_crashed_ = true;

  for (PluginInstanceSet::iterator i = instances_.begin();
       i != instances_.end(); ++i)
    (*i)->InstanceCrashed();

  if (lifetime_delegate_)
    lifetime_delegate_->PluginModuleDead(this);
}
