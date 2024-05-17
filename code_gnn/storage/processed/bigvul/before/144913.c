gfx::NativeViewAccessible RenderWidgetHostViewAura::GetNativeViewAccessible() {
#if defined(OS_WIN)
  aura::WindowTreeHost* host = window_->GetHost();
  if (!host)
    return static_cast<gfx::NativeViewAccessible>(NULL);
  BrowserAccessibilityManager* manager =
      host_->GetOrCreateRootBrowserAccessibilityManager();
  if (manager)
    return ToBrowserAccessibilityWin(manager->GetRoot());
#elif defined(OS_LINUX) && !defined(OS_CHROMEOS) && defined(USE_X11)
  BrowserAccessibilityManager* manager =
      host_->GetOrCreateRootBrowserAccessibilityManager();
  if (manager)
    return ToBrowserAccessibilityAuraLinux(manager->GetRoot())->GetAtkObject();
#endif

  NOTIMPLEMENTED();
  return static_cast<gfx::NativeViewAccessible>(NULL);
}
