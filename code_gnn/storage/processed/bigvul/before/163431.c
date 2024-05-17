void OmniboxViewViews::ExecuteCommand(int command_id, int event_flags) {
  DestroyTouchSelection();
  switch (command_id) {
    case IDS_PASTE_AND_GO:
      model()->PasteAndGo(GetClipboardText());
      return;
    case IDC_EDIT_SEARCH_ENGINES:
      location_bar_view_->command_updater()->ExecuteCommand(command_id);
      return;

    case IDS_APP_PASTE:
      ExecuteTextEditCommand(ui::TextEditCommand::PASTE);
      return;
    default:
      if (Textfield::IsCommandIdEnabled(command_id)) {
        Textfield::ExecuteCommand(command_id, event_flags);
        return;
      }
      OnBeforePossibleChange();
      location_bar_view_->command_updater()->ExecuteCommand(command_id);
      OnAfterPossibleChange(true);
      return;
  }
}
