bool ChildProcessSecurityPolicyImpl::CanCopyIntoFileSystemFile(
    int child_id,
    const storage::FileSystemURL& filesystem_url) {
  return HasPermissionsForFileSystemFile(child_id, filesystem_url,
                                         COPY_INTO_FILE_GRANT);
}
