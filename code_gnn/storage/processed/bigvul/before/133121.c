void HWNDMessageHandler::OnInitMenu(HMENU menu) {
  bool is_fullscreen = fullscreen_handler_->fullscreen();
  bool is_minimized = IsMinimized();
  bool is_maximized = IsMaximized();
  bool is_restored = !is_fullscreen && !is_minimized && !is_maximized;

  ScopedRedrawLock lock(this);
  EnableMenuItemByCommand(menu, SC_RESTORE, is_minimized || is_maximized);
  EnableMenuItemByCommand(menu, SC_MOVE, is_restored);
  EnableMenuItemByCommand(menu, SC_SIZE, delegate_->CanResize() && is_restored);
  EnableMenuItemByCommand(menu, SC_MAXIMIZE, delegate_->CanMaximize() &&
                          !is_fullscreen && !is_maximized);
  EnableMenuItemByCommand(menu, SC_MINIMIZE, delegate_->CanMaximize() &&
                          !is_minimized);

  if (is_maximized)
    ::SetMenuDefaultItem(menu, SC_RESTORE, FALSE);
  else if (delegate_->CanMaximize())
    ::SetMenuDefaultItem(menu, SC_MAXIMIZE, FALSE);
}
