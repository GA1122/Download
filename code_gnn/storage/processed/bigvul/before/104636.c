ExtensionSidebarDefaults* Extension::LoadExtensionSidebarDefaults(
    const DictionaryValue* extension_sidebar, std::string* error) {
  scoped_ptr<ExtensionSidebarDefaults> result(new ExtensionSidebarDefaults());

  std::string default_icon;
  if (extension_sidebar->HasKey(keys::kSidebarDefaultIcon)) {
    if (!extension_sidebar->GetString(keys::kSidebarDefaultIcon,
                                      &default_icon) ||
        default_icon.empty()) {
      *error = errors::kInvalidSidebarDefaultIconPath;
      return NULL;
    }
    result->set_default_icon_path(default_icon);
  }

  string16 default_title;
  if (extension_sidebar->HasKey(keys::kSidebarDefaultTitle)) {
    if (!extension_sidebar->GetString(keys::kSidebarDefaultTitle,
                                      &default_title)) {
      *error = errors::kInvalidSidebarDefaultTitle;
      return NULL;
    }
  }
  result->set_default_title(default_title);

  std::string default_page;
  if (extension_sidebar->HasKey(keys::kSidebarDefaultPage)) {
    if (!extension_sidebar->GetString(keys::kSidebarDefaultPage,
                                      &default_page) ||
        default_page.empty()) {
      *error = errors::kInvalidSidebarDefaultPage;
      return NULL;
    }
    GURL url = extension_sidebar_utils::ResolveRelativePath(
        default_page, this, error);
    if (!url.is_valid())
      return NULL;
    result->set_default_page(url);
  }

  return result.release();
}
