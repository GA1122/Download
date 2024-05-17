void OmniboxViewViews::ExecuteCommand(int command_id, int event_flags) {
  DestroyTouchSelection();
  switch (command_id) {
    case IDC_PASTE_AND_GO:
      model()->PasteAndGo(GetClipboardText());
      return;
    case IDS_SHOW_URL:
      model()->Unelide(true  );
      return;
    case IDC_EDIT_SEARCH_ENGINES:
      location_bar_view_->command_updater()->ExecuteCommand(command_id);
      return;
    case IDC_SEND_TAB_TO_SELF:
      send_tab_to_self::RecordSendTabToSelfClickResult(
          send_tab_to_self::kOmniboxMenu, SendTabToSelfClickResult::kClickItem);
      send_tab_to_self::CreateNewEntry(location_bar_view_->GetWebContents());
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
