bool BrowserView::ExecuteWindowsCommand(int command_id) {
#if defined(OS_WIN) && !defined(USE_AURA)
  if (command_id == IDC_DEBUG_FRAME_TOGGLE)
    GetWidget()->DebugToggleFrameType();

  if (base::win::IsMetroProcess()) {
    static const int sc_mask = 0xFFF0;
    if (((command_id & sc_mask) == SC_MOVE) ||
        ((command_id & sc_mask) == SC_SIZE) ||
        ((command_id & sc_mask) == SC_MAXIMIZE))
      return true;
  }
#endif
  int command_id_from_app_command = GetCommandIDForAppCommandID(command_id);
  if (command_id_from_app_command != -1)
    command_id = command_id_from_app_command;

  return chrome::ExecuteCommand(browser_.get(), command_id);
}
