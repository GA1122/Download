bool BrowserCommandController::IsReservedCommandOrKey(
    int command_id,
    const content::NativeWebKeyboardEvent& event) {
  if (browser_->is_app())
    return false;

#if defined(OS_CHROMEOS)
  ui::KeyboardCode key_code =
    static_cast<ui::KeyboardCode>(event.windowsKeyCode);
  if ((key_code == ui::VKEY_F1 && command_id == IDC_BACK) ||
      (key_code == ui::VKEY_F2 && command_id == IDC_FORWARD) ||
      (key_code == ui::VKEY_F3 && command_id == IDC_RELOAD)) {
    return true;
  }
#endif

  if (window()->IsFullscreen() && command_id == IDC_FULLSCREEN)
    return true;
  return command_id == IDC_CLOSE_TAB ||
         command_id == IDC_CLOSE_WINDOW ||
         command_id == IDC_NEW_INCOGNITO_WINDOW ||
         command_id == IDC_NEW_TAB ||
         command_id == IDC_NEW_WINDOW ||
         command_id == IDC_RESTORE_TAB ||
         command_id == IDC_SELECT_NEXT_TAB ||
         command_id == IDC_SELECT_PREVIOUS_TAB ||
         command_id == IDC_TABPOSE ||
         command_id == IDC_EXIT;
}
