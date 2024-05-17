bool ChildProcessSecurityPolicyImpl::CanDeleteFileSystemFile(
    int child_id,
    const storage::FileSystemURL& filesystem_url) {
  return HasPermissionsForFileSystemFile(child_id, filesystem_url,
                                         DELETE_FILE_GRANT);
}
