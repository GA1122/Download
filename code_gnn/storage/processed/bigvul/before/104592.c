bool Extension::EnsureNotHybridApp(const DictionaryValue* manifest,
                                   std::string* error) {
  if (web_extent().is_empty())
    return true;

  for (DictionaryValue::key_iterator key = manifest->begin_keys();
       key != manifest->end_keys(); ++key) {
    if (!IsBaseCrxKey(*key) &&
        *key != keys::kApp &&
        *key != keys::kPermissions &&
        *key != keys::kOptionsPage &&
        *key != keys::kBackground) {
      *error = ExtensionErrorUtils::FormatErrorMessage(
          errors::kHostedAppsCannotIncludeExtensionFeatures, *key);
      return false;
    }
  }

  return true;
}
