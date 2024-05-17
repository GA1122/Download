bool ChildProcessSecurityPolicyImpl::HasPermissionsForFileSystemFile(
    int child_id,
    const storage::FileSystemURL& url,
    int permissions) {
  if (!url.is_valid())
    return false;

  if (url.path().ReferencesParent())
    return false;

  if (storage::VirtualPath::IsRootPath(url.path()) &&
      (permissions & ~READ_FILE_GRANT)) {
    return false;
  }

  if (url.mount_type() == storage::kFileSystemTypeIsolated) {
    return HasPermissionsForFileSystem(
        child_id, url.mount_filesystem_id(), permissions);
  }

  FileSystemPermissionPolicyMap::iterator found =
      file_system_policy_map_.find(url.type());
  if (found == file_system_policy_map_.end())
    return false;

  if ((found->second & storage::FILE_PERMISSION_READ_ONLY) &&
      permissions & ~READ_FILE_GRANT) {
    return false;
  }

  if (found->second & storage::FILE_PERMISSION_USE_FILE_PERMISSION)
    return HasPermissionsForFile(child_id, url.path(), permissions);

  if (found->second & storage::FILE_PERMISSION_SANDBOX)
    return true;

  return false;
}
