bool Extension::LoadExtensionFeatures(APIPermissionSet* api_permissions,
                                      string16* error) {
  if (manifest_->HasKey(keys::kConvertedFromUserScript))
    manifest_->GetBoolean(keys::kConvertedFromUserScript,
                          &converted_from_user_script_);

  if (!LoadContentScripts(error) ||
      !LoadPageAction(error) ||
      !LoadSystemIndicator(api_permissions, error) ||
      !LoadIncognitoMode(error) ||
      !LoadContentSecurityPolicy(error))
    return false;

  return true;
}
