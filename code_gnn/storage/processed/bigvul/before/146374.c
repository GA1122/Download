void WebGLRenderingContextBase::SetIsHidden(bool hidden) {
  is_hidden_ = hidden;
  if (GetDrawingBuffer())
    GetDrawingBuffer()->SetIsHidden(hidden);

  if (!hidden && isContextLost() && restore_allowed_ &&
      auto_recovery_method_ == kAuto) {
    DCHECK(!restore_timer_.IsActive());
    restore_timer_.StartOneShot(0, BLINK_FROM_HERE);
  }
}
