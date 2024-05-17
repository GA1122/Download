bool ChildProcessSecurityPolicyImpl::CanReadFileSystemFile(
    int child_id,
    const storage::FileSystemURL& filesystem_url) {
  return HasPermissionsForFileSystemFile(child_id, filesystem_url,
                                         READ_FILE_GRANT);
}
