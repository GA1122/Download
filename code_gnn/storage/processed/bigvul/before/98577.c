bool CreateTabFunction::RunImpl() {
  DictionaryValue* args;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(0, &args));

  Browser *browser;
  int window_id = -1;
  if (args->HasKey(keys::kWindowIdKey)) {
    EXTENSION_FUNCTION_VALIDATE(args->GetInteger(
        keys::kWindowIdKey, &window_id));
    browser = GetBrowserInProfileWithId(profile(), window_id,
                                        include_incognito(), &error_);
  } else {
    browser = GetCurrentBrowser();
    if (!browser)
      error_ = keys::kNoCurrentWindowError;
  }
  if (!browser)
    return false;


  std::string url_string;
  GURL url;
  if (args->HasKey(keys::kUrlKey)) {
    EXTENSION_FUNCTION_VALIDATE(args->GetString(keys::kUrlKey,
                                                &url_string));
    url = ResolvePossiblyRelativeURL(url_string, GetExtension());
    if (!url.is_valid()) {
      error_ = ExtensionErrorUtils::FormatErrorMessage(keys::kInvalidUrlError,
                                                       url_string);
      return false;
    }
  }

  bool selected = true;
  if (args->HasKey(keys::kSelectedKey))
    EXTENSION_FUNCTION_VALIDATE(args->GetBoolean(keys::kSelectedKey,
                                                 &selected));
  int index = -1;
  if (args->HasKey(keys::kIndexKey))
    EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kIndexKey,
                                                 &index));

  if (url.SchemeIs(chrome::kExtensionScheme) &&
      browser->profile()->IsOffTheRecord()) {
    browser = Browser::GetOrCreateTabbedBrowser(
        browser->profile()->GetOriginalProfile());
    DCHECK(browser);
  }

  TabStripModel* tab_strip = browser->tabstrip_model();

  if (index < 0) {
    index = -1;
  }
  if (index > tab_strip->count()) {
    index = tab_strip->count();
  }

  int add_types = selected ? Browser::ADD_SELECTED : Browser::ADD_NONE;
  add_types |= Browser::ADD_FORCE_INDEX;
  TabContents* contents = browser->AddTabWithURL(url, GURL(),
      PageTransition::LINK, index, add_types, NULL, std::string());
  index = tab_strip->GetIndexOfTabContents(contents);
  browser->window()->Show();

  if (selected)
    contents->Focus();

  if (has_callback())
    result_.reset(ExtensionTabUtil::CreateTabValue(contents, tab_strip, index));

  return true;
}
