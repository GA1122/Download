void RenderWidgetHostImpl::OnLockMouse(bool user_gesture,
                                       bool last_unlocked_by_target,
                                       bool privileged) {
  if (pending_mouse_lock_request_) {
    Send(new ViewMsg_LockMouse_ACK(routing_id_, false));
    return;
  }
  if (IsMouseLocked()) {
    Send(new ViewMsg_LockMouse_ACK(routing_id_, true));
    return;
  }

  pending_mouse_lock_request_ = true;
  if (privileged && allow_privileged_mouse_lock_) {
    GotResponseToLockMouseRequest(true);
  } else {
    if (delegate_) {
      delegate_->RequestToLockMouse(this, user_gesture,
                                    last_unlocked_by_target);
      return;
    }
    GotResponseToLockMouseRequest(false);
  }
}
