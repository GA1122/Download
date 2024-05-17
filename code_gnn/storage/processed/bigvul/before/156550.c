bool ChildProcessSecurityPolicyImpl::HasPermissionsForFileSystemFile(
    int child_id,
    const storage::FileSystemURL& filesystem_url,
    int permissions) {
  if (!filesystem_url.is_valid())
    return false;

  if (filesystem_url.path().ReferencesParent())
    return false;

  if (storage::VirtualPath::IsRootPath(filesystem_url.path()) &&
      (permissions & ~READ_FILE_GRANT)) {
    return false;
  }

  if (filesystem_url.mount_type() == storage::kFileSystemTypeIsolated) {
    return HasPermissionsForFileSystem(
        child_id, filesystem_url.mount_filesystem_id(), permissions);
  }

  if (!CanCommitURL(child_id, filesystem_url.origin())) {
    UMA_HISTOGRAM_BOOLEAN("FileSystem.OriginFailedCanCommitURL", true);
    return false;
  }

  int found_permissions = 0;
  {
    base::AutoLock lock(lock_);
    FileSystemPermissionPolicyMap::iterator found =
        file_system_policy_map_.find(filesystem_url.type());
    if (found == file_system_policy_map_.end())
      return false;
    found_permissions = found->second;
  }

  if ((found_permissions & storage::FILE_PERMISSION_READ_ONLY) &&
      permissions & ~READ_FILE_GRANT) {
    return false;
  }

  if (found_permissions & storage::FILE_PERMISSION_USE_FILE_PERMISSION)
    return HasPermissionsForFile(child_id, filesystem_url.path(), permissions);

  if (found_permissions & storage::FILE_PERMISSION_SANDBOX)
    return true;

  return false;
}
