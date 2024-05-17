bool Extension::InitFromValue(int flags, string16* error) {
  DCHECK(error);

  base::AutoLock auto_lock(runtime_data_lock_);

  runtime_data_.SetActivePermissions(new PermissionSet());
  optional_permission_set_ = new PermissionSet();
  required_permission_set_ = new PermissionSet();

  creation_flags_ = flags;

  if (!LoadManifestVersion(error))
    return false;

  if (!CheckMinimumChromeVersion(error))
    return false;

  if (!LoadRequiredFeatures(error))
    return false;

  manifest_->GetString(keys::kPublicKey, &public_key_);

  extension_url_ = Extension::GetBaseURLFromExtensionId(id());

  if (is_app() && !LoadAppFeatures(error))
    return false;

  APIPermissionSet api_permissions;
  URLPatternSet host_permissions;
  if (!ParsePermissions(keys::kPermissions,
                        error,
                        &api_permissions,
                        &host_permissions)) {
    return false;
  }

  for (APIPermissionSet::const_iterator i = api_permissions.begin();
      i != api_permissions.end(); ++i) {
    if ((*i)->info()->must_be_optional()) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kPermissionMustBeOptional, (*i)->info()->name());
      return false;
    }
  }

  if (is_platform_app()) {
    api_permissions.insert(APIPermission::kAppCurrentWindowInternal);
    api_permissions.insert(APIPermission::kAppRuntime);
    api_permissions.insert(APIPermission::kAppWindow);
  }

  APIPermissionSet optional_api_permissions;
  URLPatternSet optional_host_permissions;
  if (!ParsePermissions(keys::kOptionalPermissions,
                        error,
                        &optional_api_permissions,
                        &optional_host_permissions)) {
    return false;
  }

  if (ContainsManifestForbiddenPermission(api_permissions, error) ||
      ContainsManifestForbiddenPermission(optional_api_permissions, error)) {
    return false;
  }

  if (!LoadAppIsolation(api_permissions, error))
    return false;

  if (!LoadSharedFeatures(api_permissions, error))
    return false;

  if (!LoadExtensionFeatures(&api_permissions, error))
    return false;

  if (!LoadThemeFeatures(error))
    return false;

  if (!LoadManagedModeFeatures(error))
    return false;

  if (HasMultipleUISurfaces()) {
    *error = ASCIIToUTF16(errors::kOneUISurfaceOnly);
    return false;
  }

  finished_parsing_manifest_ = true;

  runtime_data_.SetActivePermissions(new PermissionSet(
      this, api_permissions, host_permissions));
  required_permission_set_ = new PermissionSet(
      this, api_permissions, host_permissions);
  optional_permission_set_ = new PermissionSet(
      optional_api_permissions, optional_host_permissions, URLPatternSet());

  return true;
}
