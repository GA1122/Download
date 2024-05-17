bool MoveTabFunction::RunImpl() {
  int tab_id;
  EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &tab_id));
  DictionaryValue* update_props;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(1, &update_props));

  int new_index;
  EXTENSION_FUNCTION_VALIDATE(update_props->GetInteger(
      keys::kIndexKey, &new_index));
  EXTENSION_FUNCTION_VALIDATE(new_index >= 0);

  Browser* source_browser = NULL;
  TabStripModel* source_tab_strip = NULL;
  TabContents* contents = NULL;
  int tab_index = -1;
  if (!GetTabById(tab_id, profile(), include_incognito(),
                  &source_browser, &source_tab_strip, &contents,
                  &tab_index, &error_))
    return false;

  if (source_browser->type() != Browser::TYPE_NORMAL) {
    error_ = keys::kCanOnlyMoveTabsWithinNormalWindowsError;
    return false;
  }

  if (update_props->HasKey(keys::kWindowIdKey)) {
    Browser* target_browser;
    int window_id;
    EXTENSION_FUNCTION_VALIDATE(update_props->GetInteger(
        keys::kWindowIdKey, &window_id));
    target_browser = GetBrowserInProfileWithId(profile(), window_id,
                                               include_incognito(), &error_);
    if (!target_browser)
      return false;

    if (target_browser->type() != Browser::TYPE_NORMAL) {
      error_ = keys::kCanOnlyMoveTabsWithinNormalWindowsError;
      return false;
    }

    if (ExtensionTabUtil::GetWindowId(target_browser) !=
        ExtensionTabUtil::GetWindowId(source_browser)) {
      TabStripModel* target_tab_strip = target_browser->tabstrip_model();
      contents = source_tab_strip->DetachTabContentsAt(tab_index);
      if (!contents) {
        error_ = ExtensionErrorUtils::FormatErrorMessage(
            keys::kTabNotFoundError, IntToString(tab_id));
        return false;
      }

      if (new_index > target_tab_strip->count())
        new_index = target_tab_strip->count();

      target_tab_strip->InsertTabContentsAt(new_index, contents,
          false, true);

      if (has_callback())
        result_.reset(ExtensionTabUtil::CreateTabValue(contents,
            target_tab_strip, new_index));

      return true;
    }
  }

  if (new_index >= source_tab_strip->count())
    new_index = source_tab_strip->count() - 1;

  if (new_index != tab_index)
    source_tab_strip->MoveTabContentsAt(tab_index, new_index, false);

  if (has_callback())
    result_.reset(ExtensionTabUtil::CreateTabValue(contents, source_tab_strip,
        new_index));
  return true;
}
