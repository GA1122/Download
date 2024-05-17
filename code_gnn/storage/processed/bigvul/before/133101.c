void HWNDMessageHandler::LockUpdates(bool force) {
  if ((force || !ui::win::IsAeroGlassEnabled()) && ++lock_updates_count_ == 1) {
    SetWindowLong(hwnd(), GWL_STYLE,
                  GetWindowLong(hwnd(), GWL_STYLE) & ~WS_VISIBLE);
  }
}
