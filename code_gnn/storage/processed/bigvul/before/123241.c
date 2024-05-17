void RenderWidgetHostViewAura::SetBounds(const gfx::Rect& rect) {
  if (window_->bounds().size() != rect.size() &&
      host_->is_accelerated_compositing_active()) {
    aura::RootWindow* root_window = window_->GetRootWindow();
    ui::Compositor* compositor = root_window ?
        root_window->compositor() : NULL;
    if (root_window && compositor) {
      if (!compositor->HasObserver(this))
        compositor->AddObserver(this);

      bool defer_compositor_lock =
         can_lock_compositor_ == NO_PENDING_RENDERER_FRAME ||
         can_lock_compositor_ == NO_PENDING_COMMIT;

      if (can_lock_compositor_ == YES)
        can_lock_compositor_ = YES_DID_LOCK;

      resize_locks_.push_back(make_linked_ptr(
          new ResizeLock(root_window, rect.size(), defer_compositor_lock)));
    }
  }
  window_->SetBounds(rect);
  host_->WasResized();
}
