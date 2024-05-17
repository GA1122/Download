bool Extension::LoadBackgroundPersistent(
    const APIPermissionSet& api_permissions,
    string16* error) {
  if (is_platform_app()) {
    background_page_is_persistent_ = false;
    return true;
  }

  Value* background_persistent = NULL;
  if (!manifest_->Get(keys::kBackgroundPersistent, &background_persistent))
    return true;

  if (!background_persistent->GetAsBoolean(&background_page_is_persistent_)) {
    *error = ASCIIToUTF16(errors::kInvalidBackgroundPersistent);
    return false;
  }

  if (!has_background_page()) {
    *error = ASCIIToUTF16(errors::kInvalidBackgroundPersistentNoPage);
    return false;
  }

  return true;
}
