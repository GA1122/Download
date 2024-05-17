void RenderWidgetHostImpl::OnUnlockMouse() {
  const bool was_mouse_locked = !pending_mouse_lock_request_ && IsMouseLocked();
  RejectMouseLockOrUnlockIfNecessary();
  if (was_mouse_locked)
    is_last_unlocked_by_target_ = true;
}
