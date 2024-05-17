bool TabsRemoveFunction::RemoveTab(int tab_id, std::string* error) {
  Browser* browser = NULL;
  WebContents* contents = NULL;
  if (!GetTabById(tab_id, browser_context(), include_incognito(), &browser,
                  nullptr, &contents, nullptr, error)) {
    return false;
  }

  if (!browser->window()->IsTabStripEditable()) {
    *error = keys::kTabStripNotEditableError;
    return false;
  }
  contents->Close();
  return true;
}
