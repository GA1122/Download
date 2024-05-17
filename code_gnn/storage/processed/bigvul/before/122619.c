void Extension::ClearTabSpecificPermissions(int tab_id) const {
  base::AutoLock auto_lock(runtime_data_lock_);
  runtime_data_.ClearTabSpecificPermissions(tab_id);
}
