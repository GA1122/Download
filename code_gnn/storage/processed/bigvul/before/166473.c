bool UnpackAPIPermissions(const std::vector<std::string>& permissions_input,
                          const PermissionSet& required_permissions,
                          const PermissionSet& optional_permissions,
                          UnpackPermissionSetResult* result,
                          std::string* error) {
  PermissionsInfo* info = PermissionsInfo::GetInstance();
  APIPermissionSet apis;
  for (const auto& permission_str : permissions_input) {
    size_t delimiter = permission_str.find(kDelimiter);
    if (delimiter != std::string::npos) {
      base::StringPiece permission_piece(permission_str);
      std::unique_ptr<APIPermission> permission = UnpackPermissionWithArguments(
          permission_piece.substr(0, delimiter),
          permission_piece.substr(delimiter + 1), permission_str, error);
      if (!permission)
        return false;

      apis.insert(std::move(permission));
    } else {
      const APIPermissionInfo* permission_info =
          info->GetByName(permission_str);
      if (!permission_info) {
        *error = ErrorUtils::FormatErrorMessage(kUnknownPermissionError,
                                                permission_str);
        return false;
      }
      apis.insert(permission_info->id());
    }
  }

  for (const auto* api_permission : apis) {
    if (required_permissions.apis().count(api_permission->id())) {
      result->required_apis.insert(api_permission->id());
      continue;
    }

    if (!optional_permissions.apis().count(api_permission->id())) {
      result->unlisted_apis.insert(api_permission->id());
      continue;
    }

    if (!api_permission->info()->supports_optional()) {
      result->unsupported_optional_apis.insert(api_permission->id());
      continue;
    }

    result->optional_apis.insert(api_permission->id());
  }

  return true;
}
