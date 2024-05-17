gfx::Rect RenderWidgetHostViewAura::GetBoundsInRootWindow() {
#if defined(OS_WIN)
  RECT window_rect = {0};

  aura::Window* top_level = window_->GetToplevelWindow();
  aura::WindowEventDispatcher* dispatcher = top_level->GetDispatcher();
  if (!dispatcher)
    return top_level->GetBoundsInScreen();
  HWND hwnd = dispatcher->host()->GetAcceleratedWidget();
  ::GetWindowRect(hwnd, &window_rect);
  gfx::Rect rect(window_rect);

  if (::IsZoomed(hwnd)) {
    rect.Inset(GetSystemMetrics(SM_CXSIZEFRAME),
               GetSystemMetrics(SM_CYSIZEFRAME));
  }

  return gfx::win::ScreenToDIPRect(rect);
#else
  return window_->GetToplevelWindow()->GetBoundsInScreen();
#endif
}
