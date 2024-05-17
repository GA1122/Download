bool Extension::LoadAppIsolation(const APIPermissionSet& api_permissions,
                                 string16* error) {
  if (is_platform_app()) {
    is_storage_isolated_ = true;
    return true;
  }

  if (!api_permissions.count(APIPermission::kExperimental) || !is_app())
    return true;

  Value* tmp_isolation = NULL;
  if (!manifest_->Get(keys::kIsolation, &tmp_isolation))
    return true;

  if (tmp_isolation->GetType() != Value::TYPE_LIST) {
    *error = ASCIIToUTF16(errors::kInvalidIsolation);
    return false;
  }

  ListValue* isolation_list = static_cast<ListValue*>(tmp_isolation);
  for (size_t i = 0; i < isolation_list->GetSize(); ++i) {
    std::string isolation_string;
    if (!isolation_list->GetString(i, &isolation_string)) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kInvalidIsolationValue,
          base::UintToString(i));
      return false;
    }

    if (isolation_string == values::kIsolatedStorage) {
      is_storage_isolated_ = true;
    } else {
      DLOG(WARNING) << "Did not recognize isolation type: " << isolation_string;
    }
  }
  return true;
}
