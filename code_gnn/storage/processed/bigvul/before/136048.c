  void GrantPermissionsForFile(ChildProcessSecurityPolicyImpl* p,
                               int child_id,
                               const base::FilePath& file,
                               int permissions) {
    p->GrantPermissionsForFile(child_id, file, permissions);
  }
