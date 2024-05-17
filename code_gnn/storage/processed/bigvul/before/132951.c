gfx::NativeViewAccessible RenderWidgetHostViewAura::GetNativeViewAccessible() {
#if defined(OS_WIN)
  aura::WindowEventDispatcher* dispatcher = window_->GetDispatcher();
  if (!dispatcher)
    return static_cast<gfx::NativeViewAccessible>(NULL);
  HWND hwnd = dispatcher->host()->GetAcceleratedWidget();

  CreateBrowserAccessibilityManagerIfNeeded();
  BrowserAccessibilityManager* manager = GetBrowserAccessibilityManager();
  if (manager)
    return manager->GetRoot()->ToBrowserAccessibilityWin();
#endif

  NOTIMPLEMENTED();
  return static_cast<gfx::NativeViewAccessible>(NULL);
}
