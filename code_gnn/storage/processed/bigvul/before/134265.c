bool OmniboxViewViews::IsCommandIdEnabled(int command_id) const {
  if (command_id == IDS_APP_PASTE)
    return !read_only() && !GetClipboardText().empty();
  if (command_id == IDS_PASTE_AND_GO)
    return !read_only() && model()->CanPasteAndGo(GetClipboardText());
  if (command_id == IDS_SHOW_URL)
    return controller()->GetToolbarModel()->WouldReplaceURL();
  return Textfield::IsCommandIdEnabled(command_id) ||
         command_updater()->IsCommandEnabled(command_id);
}
