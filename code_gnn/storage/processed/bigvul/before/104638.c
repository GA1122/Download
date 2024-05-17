FileBrowserHandler* Extension::LoadFileBrowserHandler(
    const DictionaryValue* file_browser_handler, std::string* error) {
  scoped_ptr<FileBrowserHandler> result(
      new FileBrowserHandler());
  result->set_extension_id(id());

  std::string id;
  if (!file_browser_handler->HasKey(keys::kPageActionId) ||
      !file_browser_handler->GetString(keys::kPageActionId, &id)) {
    *error = errors::kInvalidPageActionId;
    return NULL;
  }
  result->set_id(id);

  std::string title;
  if (!file_browser_handler->HasKey(keys::kPageActionDefaultTitle) ||
      !file_browser_handler->GetString(keys::kPageActionDefaultTitle, &title)) {
    *error = errors::kInvalidPageActionDefaultTitle;
    return NULL;
  }
  result->set_title(title);

  ListValue* list_value = NULL;
  if (!file_browser_handler->HasKey(keys::kFileFilters) ||
      !file_browser_handler->GetList(keys::kFileFilters, &list_value) ||
      list_value->empty()) {
    *error = errors::kInvalidFileFiltersList;
    return NULL;
  }
  for (size_t i = 0; i < list_value->GetSize(); ++i) {
    std::string filter;
    if (!list_value->GetString(i, &filter)) {
      *error = ExtensionErrorUtils::FormatErrorMessage(
          errors::kInvalidFileFilterValue, base::IntToString(i));
      return NULL;
    }
    URLPattern pattern(URLPattern::SCHEME_FILESYSTEM);
    if (pattern.Parse(filter, URLPattern::ERROR_ON_PORTS) !=
        URLPattern::PARSE_SUCCESS) {
      *error = ExtensionErrorUtils::FormatErrorMessage(
          errors::kInvalidURLPatternError, filter);
      return NULL;
    }
    result->AddPattern(pattern);
  }

  std::string default_icon;
  if (file_browser_handler->HasKey(keys::kPageActionDefaultIcon)) {
    if (!file_browser_handler->GetString(
            keys::kPageActionDefaultIcon,&default_icon) ||
        default_icon.empty()) {
      *error = errors::kInvalidPageActionIconPath;
      return NULL;
    }
    result->set_icon_path(default_icon);
  }

  return result.release();
}
