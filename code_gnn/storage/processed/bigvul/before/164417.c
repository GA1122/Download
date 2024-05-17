bool PermissionsData::HasHostPermission(const GURL& url) const {
  base::AutoLock auto_lock(runtime_lock_);
  return active_permissions_unsafe_->HasExplicitAccessToOrigin(url) &&
         !IsPolicyBlockedHostUnsafe(url);
}
