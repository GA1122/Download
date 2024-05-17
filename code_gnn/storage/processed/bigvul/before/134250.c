void OmniboxViewViews::ExecuteCommand(int command_id, int event_flags) {
  switch (command_id) {
    case IDS_PASTE_AND_GO:
      model()->PasteAndGo(GetClipboardText());
      break;
    case IDS_SHOW_URL:
      ShowURL();
      break;
    case IDC_EDIT_SEARCH_ENGINES:
      command_updater()->ExecuteCommand(command_id);
      break;

    default:
      OnBeforePossibleChange();
      if (command_id == IDS_APP_PASTE)
        OnPaste();
      else if (Textfield::IsCommandIdEnabled(command_id))
        Textfield::ExecuteCommand(command_id, event_flags);
      else
        command_updater()->ExecuteCommand(command_id);
      OnAfterPossibleChange();
      break;
  }
}
