bool Extension::LoadNaClModules(string16* error) {
  if (!manifest_->HasKey(keys::kNaClModules))
    return true;
  ListValue* list_value = NULL;
  if (!manifest_->GetList(keys::kNaClModules, &list_value)) {
    *error = ASCIIToUTF16(errors::kInvalidNaClModules);
    return false;
  }

  for (size_t i = 0; i < list_value->GetSize(); ++i) {
    DictionaryValue* module_value = NULL;
    if (!list_value->GetDictionary(i, &module_value)) {
      *error = ASCIIToUTF16(errors::kInvalidNaClModules);
      return false;
    }

    std::string path_str;
    if (!module_value->GetString(keys::kNaClModulesPath, &path_str)) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kInvalidNaClModulesPath, base::IntToString(i));
      return false;
    }

    std::string mime_type;
    if (!module_value->GetString(keys::kNaClModulesMIMEType, &mime_type)) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kInvalidNaClModulesMIMEType, base::IntToString(i));
      return false;
    }

    nacl_modules_.push_back(NaClModuleInfo());
    nacl_modules_.back().url = GetResourceURL(path_str);
    nacl_modules_.back().mime_type = mime_type;
  }

  return true;
}
