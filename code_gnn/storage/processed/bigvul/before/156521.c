bool ChildProcessSecurityPolicyImpl::CanCreateFileSystemFile(
    int child_id,
    const storage::FileSystemURL& filesystem_url) {
  return HasPermissionsForFileSystemFile(child_id, filesystem_url,
                                         CREATE_NEW_FILE_GRANT);
}
