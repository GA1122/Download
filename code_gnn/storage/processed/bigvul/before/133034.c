bool RenderWidgetHostViewAura::ShouldCreateResizeLock() {
#if defined (OS_WIN)
  return false;
#endif

  if (resize_lock_)
    return false;

  if (host_->should_auto_resize())
    return false;
  if (!host_->is_accelerated_compositing_active())
    return false;

  gfx::Size desired_size = window_->bounds().size();
  if (desired_size == current_frame_size_)
    return false;

  aura::WindowEventDispatcher* dispatcher = window_->GetDispatcher();
  if (!dispatcher)
    return false;

  ui::Compositor* compositor = dispatcher->host()->compositor();
  if (!compositor)
    return false;

  return true;
}
