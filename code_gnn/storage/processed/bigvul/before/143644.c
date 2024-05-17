bool RenderWidgetHostImpl::GotResponseToLockMouseRequest(bool allowed) {
  if (!allowed) {
    RejectMouseLockOrUnlockIfNecessary();
    return false;
  }

  if (!pending_mouse_lock_request_) {
    return false;
  }

  pending_mouse_lock_request_ = false;
  if (!view_ || !view_->HasFocus()|| !view_->LockMouse()) {
    Send(new ViewMsg_LockMouse_ACK(routing_id_, false));
    return false;
  }

  Send(new ViewMsg_LockMouse_ACK(routing_id_, true));
  return true;
}
