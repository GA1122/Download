  void GrantPermissionsForFileSystem(const std::string& filesystem_id,
                                     int permissions) {
    if (!base::ContainsKey(filesystem_permissions_, filesystem_id))
      storage::IsolatedContext::GetInstance()->AddReference(filesystem_id);
    filesystem_permissions_[filesystem_id] |= permissions;
  }
