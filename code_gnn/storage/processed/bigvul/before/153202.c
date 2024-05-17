void DesktopWindowTreeHostX11::ClearNativeFocus() {
  if (content_window() && aura::client::GetFocusClient(content_window()) &&
      content_window()->Contains(
          aura::client::GetFocusClient(content_window())->GetFocusedWindow())) {
    aura::client::GetFocusClient(content_window())
        ->FocusWindow(content_window());
  }
}
