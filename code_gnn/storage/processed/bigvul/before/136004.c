bool ChildProcessSecurityPolicyImpl::CanReadFileSystemFile(
    int child_id,
    const storage::FileSystemURL& url) {
  return HasPermissionsForFileSystemFile(child_id, url, READ_FILE_GRANT);
}
