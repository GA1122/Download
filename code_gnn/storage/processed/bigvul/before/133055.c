void RenderWidgetHostViewAura::WasHidden() {
  if (!host_ || host_->is_hidden())
    return;
  host_->WasHidden();
  software_frame_manager_->SetVisibility(false);
  delegated_frame_evictor_->SetVisible(false);
  released_front_lock_ = NULL;

#if defined(OS_WIN)
  constrained_rects_.clear();
  aura::WindowEventDispatcher* dispatcher = window_->GetDispatcher();
  if (dispatcher) {
    HWND parent = dispatcher->host()->GetAcceleratedWidget();
    LPARAM lparam = reinterpret_cast<LPARAM>(this);

    EnumChildWindows(parent, HideWindowsCallback, lparam);
  }
  if (::IsWindow(plugin_parent_window_))
    ::SetWindowPos(plugin_parent_window_, NULL, 0, 0, 0, 0, 0);
#endif
}
