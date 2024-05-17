bool Extension::LoadSystemIndicator(APIPermissionSet* api_permissions,
                                    string16* error) {
  if (!manifest_->HasKey(keys::kSystemIndicator)) {
    return true;
  }

  DictionaryValue* system_indicator_value = NULL;
  if (!manifest_->GetDictionary(keys::kSystemIndicator,
                                &system_indicator_value)) {
    *error = ASCIIToUTF16(errors::kInvalidSystemIndicator);
    return false;
  }

  system_indicator_info_ = LoadExtensionActionInfoHelper(
      this, system_indicator_value, error);

  if (!system_indicator_info_.get()) {
    return false;
  }

  api_permissions->insert(APIPermission::kSystemIndicator);

  return true;
}
