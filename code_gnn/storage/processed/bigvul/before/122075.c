APIPermissionInfo::APIPermissionInfo(
    APIPermission::ID id,
    const char* name,
    int l10n_message_id,
    PermissionMessage::ID message_id,
    int flags,
    APIPermissionConstructor api_permission_constructor)
    : id_(id),
      name_(name),
      flags_(flags),
      l10n_message_id_(l10n_message_id),
      message_id_(message_id),
      api_permission_constructor_(api_permission_constructor) { }
