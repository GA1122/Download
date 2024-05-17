bool TaskManagerView::ExecuteWindowsCommand(int command_id) {
#if defined(OS_WIN) && !defined(USE_AURA)
  if (command_id == IDC_ALWAYS_ON_TOP) {
    is_always_on_top_ = !is_always_on_top_;

    HMENU system_menu = GetSystemMenu(GetWidget()->GetNativeWindow(), FALSE);
    MENUITEMINFO menu_info;
    memset(&menu_info, 0, sizeof(MENUITEMINFO));
    menu_info.cbSize = sizeof(MENUITEMINFO);
    BOOL r = GetMenuItemInfo(system_menu, IDC_ALWAYS_ON_TOP,
                             FALSE, &menu_info);
    DCHECK(r);
    menu_info.fMask = MIIM_STATE;
    if (is_always_on_top_)
      menu_info.fState = MFS_CHECKED;
    r = SetMenuItemInfo(system_menu, IDC_ALWAYS_ON_TOP, FALSE, &menu_info);

    GetWidget()->SetAlwaysOnTop(is_always_on_top_);

    if (g_browser_process->local_state()) {
      DictionaryPrefUpdate update(g_browser_process->local_state(),
                                  GetWindowName().c_str());
      DictionaryValue* window_preferences = update.Get();
      window_preferences->SetBoolean("always_on_top", is_always_on_top_);
    }
    return true;
  }
#endif
  return false;
}
