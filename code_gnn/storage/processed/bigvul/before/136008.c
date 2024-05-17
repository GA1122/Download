bool ChildProcessSecurityPolicyImpl::CanWriteFileSystemFile(
    int child_id,
    const storage::FileSystemURL& url) {
  return HasPermissionsForFileSystemFile(child_id, url, WRITE_FILE_GRANT);
}
