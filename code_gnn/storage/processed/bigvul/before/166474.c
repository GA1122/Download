std::unique_ptr<UnpackPermissionSetResult> UnpackPermissionSet(
    const Permissions& permissions_input,
    const PermissionSet& required_permissions,
    const PermissionSet& optional_permissions,
    bool allow_file_access,
    std::string* error) {
  DCHECK(error);


  auto result = std::make_unique<UnpackPermissionSetResult>();

  if (permissions_input.permissions &&
      !UnpackAPIPermissions(*permissions_input.permissions,
                            required_permissions, optional_permissions,
                            result.get(), error)) {
    return nullptr;
  }

  if (permissions_input.origins &&
      !UnpackOriginPermissions(*permissions_input.origins, required_permissions,
                               optional_permissions, allow_file_access,
                               result.get(), error)) {
    return nullptr;
  }

  return result;
}
