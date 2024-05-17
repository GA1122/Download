bool ChildProcessSecurityPolicyImpl::CanWriteFileSystemFile(
    int child_id,
    const storage::FileSystemURL& filesystem_url) {
  return HasPermissionsForFileSystemFile(child_id, filesystem_url,
                                         WRITE_FILE_GRANT);
}
