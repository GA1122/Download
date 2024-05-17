void RenderWidgetHostViewAura::CreateBrowserAccessibilityManagerIfNeeded() {
  if (GetBrowserAccessibilityManager())
    return;

#if defined(OS_WIN)
  aura::WindowEventDispatcher* dispatcher = window_->GetDispatcher();
  if (!dispatcher)
    return;
  HWND hwnd = dispatcher->host()->GetAcceleratedWidget();

  gfx::NativeViewAccessible accessible_parent =
      host_->GetParentNativeViewAccessible();

  BrowserAccessibilityManager* manager = new BrowserAccessibilityManagerWin(
      hwnd, accessible_parent,
      BrowserAccessibilityManagerWin::GetEmptyDocument(), this);
#else
  BrowserAccessibilityManager* manager = BrowserAccessibilityManager::Create(
      BrowserAccessibilityManager::GetEmptyDocument(), this);
#endif

  SetBrowserAccessibilityManager(manager);
}
