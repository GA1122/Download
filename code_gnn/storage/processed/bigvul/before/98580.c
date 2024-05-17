bool UpdateTabFunction::RunImpl() {
  int tab_id;
  EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &tab_id));
  DictionaryValue* update_props;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(1, &update_props));

  TabStripModel* tab_strip = NULL;
  TabContents* contents = NULL;
  int tab_index = -1;
  if (!GetTabById(tab_id, profile(), include_incognito(),
                  NULL, &tab_strip, &contents, &tab_index, &error_))
    return false;

  NavigationController& controller = contents->controller();


  std::string url_string;
  if (update_props->HasKey(keys::kUrlKey)) {
    EXTENSION_FUNCTION_VALIDATE(update_props->GetString(
        keys::kUrlKey, &url_string));
    GURL url = ResolvePossiblyRelativeURL(url_string, GetExtension());

    if (!url.is_valid()) {
      error_ = ExtensionErrorUtils::FormatErrorMessage(keys::kInvalidUrlError,
                                                       url_string);
      return false;
    }

    if (url.SchemeIs(chrome::kJavaScriptScheme)) {
      if (!GetExtension()->CanExecuteScriptOnHost(contents->GetURL(), &error_))
        return false;

    }

    if (tab_strip->IsTabPinned(tab_index)) {
      error_ = keys::kCannotUpdatePinnedTab;
      return false;
    }

    controller.LoadURL(url, GURL(), PageTransition::LINK);

    if (!url.SchemeIs(chrome::kJavaScriptScheme))
      DCHECK_EQ(url.spec(), contents->GetURL().spec());
  }

  bool selected = false;
  if (update_props->HasKey(keys::kSelectedKey)) {
    EXTENSION_FUNCTION_VALIDATE(update_props->GetBoolean(
        keys::kSelectedKey,
        &selected));
    if (selected) {
      if (tab_strip->selected_index() != tab_index) {
        tab_strip->SelectTabContentsAt(tab_index, false);
        DCHECK_EQ(contents, tab_strip->GetSelectedTabContents());
      }
      contents->Focus();
    }
  }

  if (has_callback())
    result_.reset(ExtensionTabUtil::CreateTabValue(contents, tab_strip,
        tab_index));

  return true;
}
