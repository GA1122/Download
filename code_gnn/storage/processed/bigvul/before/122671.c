bool Extension::LoadBackgroundPage(
    const APIPermissionSet& api_permissions,
    string16* error) {
  if (is_platform_app()) {
    return LoadBackgroundPage(
        keys::kPlatformAppBackgroundPage, api_permissions, error);
  }

  if (!LoadBackgroundPage(keys::kBackgroundPage, api_permissions, error))
    return false;
  if (background_url_.is_empty()) {
    return LoadBackgroundPage(
        keys::kBackgroundPageLegacy, api_permissions, error);
  }
  return true;
}
