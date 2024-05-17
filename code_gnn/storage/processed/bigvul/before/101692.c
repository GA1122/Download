bool Browser::IsReservedCommandOrKey(int command_id,
                                     const NativeWebKeyboardEvent& event) {
#if defined(OS_CHROMEOS)
  ui::KeyboardCode key_code =
      static_cast<ui::KeyboardCode>(event.windowsKeyCode);
  if (key_code == ui::VKEY_F1 ||
      key_code == ui::VKEY_F2 ||
      key_code == ui::VKEY_F3 ||
      key_code == ui::VKEY_F4 ||
      key_code == ui::VKEY_F5 ||
      key_code == ui::VKEY_F6 ||
      key_code == ui::VKEY_F7 ||
      key_code == ui::VKEY_F8 ||
      key_code == ui::VKEY_F9 ||
      key_code == ui::VKEY_F10) {
    return true;
  }
#endif

  return command_id == IDC_CLOSE_TAB ||
         command_id == IDC_CLOSE_WINDOW ||
         command_id == IDC_NEW_INCOGNITO_WINDOW ||
         command_id == IDC_NEW_TAB ||
         command_id == IDC_NEW_WINDOW ||
         command_id == IDC_RESTORE_TAB ||
         command_id == IDC_SELECT_NEXT_TAB ||
         command_id == IDC_SELECT_PREVIOUS_TAB ||
         command_id == IDC_TABPOSE ||
         command_id == IDC_EXIT ||
         command_id == IDC_SEARCH;
}
