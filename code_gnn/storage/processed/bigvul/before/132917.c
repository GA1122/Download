void RenderWidgetHostViewAura::CheckResizeLock() {
  if (!resize_lock_ || resize_lock_->expected_size() != current_frame_size_)
    return;

  resize_lock_->UnlockCompositor();
  ui::Compositor* compositor = GetCompositor();
  if (compositor) {
    if (!compositor->HasObserver(this))
      compositor->AddObserver(this);
  }
}
