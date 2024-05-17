bool ChildProcessSecurityPolicyImpl::CanCopyIntoFileSystemFile(
    int child_id,
    const storage::FileSystemURL& url) {
  return HasPermissionsForFileSystemFile(child_id, url, COPY_INTO_FILE_GRANT);
}
