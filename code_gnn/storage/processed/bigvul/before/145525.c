void RemoveDownloadFileFromChildSecurityPolicy(int child_id,
                                               const base::FilePath& path) {
  ChildProcessSecurityPolicyImpl::GetInstance()->RevokeAllPermissionsForFile(
      child_id, path);
}
