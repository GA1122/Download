bool TabsMoveFunction::MoveTab(int tab_id,
                               int* new_index,
                               int iteration,
                               base::ListValue* tab_values,
                               int* window_id,
                               std::string* error) {
  Browser* source_browser = NULL;
  TabStripModel* source_tab_strip = NULL;
  WebContents* contents = NULL;
  int tab_index = -1;
  if (!GetTabById(tab_id, browser_context(), include_incognito(),
                  &source_browser, &source_tab_strip, &contents, &tab_index,
                  error)) {
    return false;
  }

  if (!source_browser->window()->IsTabStripEditable()) {
    *error = keys::kTabStripNotEditableError;
    return false;
  }

  *new_index += iteration;

  if (window_id) {
    Browser* target_browser = NULL;

    if (!GetBrowserFromWindowID(this, *window_id, &target_browser, error))
      return false;

    if (!target_browser->window()->IsTabStripEditable()) {
      *error = keys::kTabStripNotEditableError;
      return false;
    }

    if (!target_browser->is_type_tabbed()) {
      *error = keys::kCanOnlyMoveTabsWithinNormalWindowsError;
      return false;
    }

    if (target_browser->profile() != source_browser->profile()) {
      *error = keys::kCanOnlyMoveTabsWithinSameProfileError;
      return false;
    }

    if (ExtensionTabUtil::GetWindowId(target_browser) !=
        ExtensionTabUtil::GetWindowId(source_browser)) {
      TabStripModel* target_tab_strip = target_browser->tab_strip_model();
      WebContents* web_contents =
          source_tab_strip->DetachWebContentsAt(tab_index);
      if (!web_contents) {
        *error = ErrorUtils::FormatErrorMessage(keys::kTabNotFoundError,
                                                base::IntToString(tab_id));
        return false;
      }

      if (*new_index > target_tab_strip->count() || *new_index < 0)
        *new_index = target_tab_strip->count();

      target_tab_strip->InsertWebContentsAt(
          *new_index, web_contents, TabStripModel::ADD_NONE);

      if (has_callback()) {
        tab_values->Append(ExtensionTabUtil::CreateTabObject(
                               web_contents, ExtensionTabUtil::kScrubTab,
                               extension(), target_tab_strip, *new_index)
                               ->ToValue());
      }

      return true;
    }
  }

  if (*new_index >= source_tab_strip->count() || *new_index < 0)
    *new_index = source_tab_strip->count() - 1;

  if (*new_index != tab_index)
    source_tab_strip->MoveWebContentsAt(tab_index, *new_index, false);

  if (has_callback()) {
    tab_values->Append(ExtensionTabUtil::CreateTabObject(
                           contents, ExtensionTabUtil::kScrubTab, extension(),
                           source_tab_strip, *new_index)
                           ->ToValue());
  }

  return true;
}
