bool ChildProcessSecurityPolicyImpl::CanCreateReadWriteFileSystemFile(
    int child_id,
    const storage::FileSystemURL& filesystem_url) {
  return HasPermissionsForFileSystemFile(child_id, filesystem_url,
                                         CREATE_READ_WRITE_FILE_GRANT);
}
