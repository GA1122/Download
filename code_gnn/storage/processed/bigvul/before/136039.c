void ChildProcessSecurityPolicyImpl::RegisterFileSystemPermissionPolicy(
    storage::FileSystemType type,
    int policy) {
  base::AutoLock lock(lock_);
  file_system_policy_map_[type] = policy;
}
