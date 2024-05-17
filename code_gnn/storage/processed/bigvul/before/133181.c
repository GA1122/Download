void HWNDMessageHandler::UnlockUpdates(bool force) {
  if ((force || !ui::win::IsAeroGlassEnabled()) && --lock_updates_count_ <= 0) {
    SetWindowLong(hwnd(), GWL_STYLE,
                  GetWindowLong(hwnd(), GWL_STYLE) | WS_VISIBLE);
    lock_updates_count_ = 0;
  }
}
