bool Extension::LoadPlugins(string16* error) {
  if (!manifest_->HasKey(keys::kPlugins))
    return true;

  ListValue* list_value = NULL;
  if (!manifest_->GetList(keys::kPlugins, &list_value)) {
    *error = ASCIIToUTF16(errors::kInvalidPlugins);
    return false;
  }

  for (size_t i = 0; i < list_value->GetSize(); ++i) {
    DictionaryValue* plugin_value = NULL;
    if (!list_value->GetDictionary(i, &plugin_value)) {
      *error = ASCIIToUTF16(errors::kInvalidPlugins);
      return false;
    }
    std::string path_str;
    if (!plugin_value->GetString(keys::kPluginsPath, &path_str)) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kInvalidPluginsPath, base::IntToString(i));
      return false;
    }

    bool is_public = false;
    if (plugin_value->HasKey(keys::kPluginsPublic)) {
      if (!plugin_value->GetBoolean(keys::kPluginsPublic, &is_public)) {
        *error = ErrorUtils::FormatErrorMessageUTF16(
            errors::kInvalidPluginsPublic, base::IntToString(i));
        return false;
      }
    }

#if defined(OS_CHROMEOS)
    continue;
#elif defined(OS_WIN)
    if (base::win::IsMetroProcess()) {
      continue;
    }
#endif   
    plugins_.push_back(PluginInfo());
    plugins_.back().path = path().Append(FilePath::FromUTF8Unsafe(path_str));
    plugins_.back().is_public = is_public;
  }
  return true;
}
