gfx::Rect RenderWidgetHostViewAura::GetBoundsInRootWindow() {
  aura::Window* top_level = window_->GetToplevelWindow();
  gfx::Rect bounds(top_level->GetBoundsInScreen());

#if defined(OS_WIN)
  if (UsesNativeWindowFrame()) {
    aura::WindowTreeHost* host = top_level->GetHost();
    if (!host)
      return top_level->GetBoundsInScreen();
    RECT window_rect = {0};
    HWND hwnd = host->GetAcceleratedWidget();
    ::GetWindowRect(hwnd, &window_rect);
    bounds = gfx::Rect(window_rect);

    if (::IsZoomed(hwnd)) {
      bounds.Inset(GetSystemMetrics(SM_CXSIZEFRAME),
                   GetSystemMetrics(SM_CYSIZEFRAME));

      bounds.Inset(GetSystemMetrics(SM_CXPADDEDBORDER),
                   GetSystemMetrics(SM_CXPADDEDBORDER));
    }
  }

  bounds =
      display::Screen::GetScreen()->ScreenToDIPRectInWindow(top_level, bounds);
#endif

  return bounds;
}
