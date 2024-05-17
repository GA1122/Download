void RenderWidgetHostViewAura::WasShown() {
  DCHECK(host_);
  if (!host_->is_hidden())
    return;
  host_->WasShown();
  software_frame_manager_->SetVisibility(true);
  delegated_frame_evictor_->SetVisible(true);

  aura::Window* root = window_->GetRootWindow();
  if (root) {
    aura::client::CursorClient* cursor_client =
        aura::client::GetCursorClient(root);
    if (cursor_client)
      NotifyRendererOfCursorVisibilityState(cursor_client->IsCursorVisible());
  }

  if (!current_surface_.get() && host_->is_accelerated_compositing_active() &&
      !released_front_lock_.get()) {
    ui::Compositor* compositor = GetCompositor();
    if (compositor)
      released_front_lock_ = compositor->GetCompositorLock();
  }

#if defined(OS_WIN)
  LPARAM lparam = reinterpret_cast<LPARAM>(this);
  EnumChildWindows(ui::GetHiddenWindow(), ShowWindowsCallback, lparam);

  if (::IsWindow(plugin_parent_window_)) {
    gfx::Rect window_bounds = window_->GetBoundsInRootWindow();
    ::SetWindowPos(plugin_parent_window_, NULL, window_bounds.x(),
                   window_bounds.y(), window_bounds.width(),
                   window_bounds.height(), 0);
  }
#endif
}
