void RenderWidgetHostViewAura::MaybeCreateResizeLock() {
  if (!ShouldCreateResizeLock())
    return;
  DCHECK(window_->GetDispatcher());
  DCHECK(window_->GetDispatcher()->host()->compositor());

  ui::Compositor* compositor = window_->GetDispatcher()->host()->compositor();
  if (!compositor->HasObserver(this))
    compositor->AddObserver(this);

  bool defer_compositor_lock =
      can_lock_compositor_ == NO_PENDING_RENDERER_FRAME ||
      can_lock_compositor_ == NO_PENDING_COMMIT;

  if (can_lock_compositor_ == YES)
    can_lock_compositor_ = YES_DID_LOCK;

  resize_lock_ = CreateResizeLock(defer_compositor_lock);
}
