void RemoveDownloadFileFromChildSecurityPolicy(int child_id,
                                               const FilePath& path) {
  ChildProcessSecurityPolicyImpl::GetInstance()->RevokeAllPermissionsForFile(
      child_id, path);
}
