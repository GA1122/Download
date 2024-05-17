bool PermissionsData::HasEffectiveAccessToAllHosts() const {
  base::AutoLock auto_lock(runtime_lock_);
  return active_permissions_unsafe_->HasEffectiveAccessToAllHosts();
}
