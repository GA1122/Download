void RenderWidgetHostImpl::OnLockMouse(bool user_gesture,
                                       bool privileged) {
  if (pending_mouse_lock_request_) {
    Send(new ViewMsg_LockMouse_ACK(routing_id_, false));
    return;
  }

  pending_mouse_lock_request_ = true;
  if (delegate_) {
    delegate_->RequestToLockMouse(this, user_gesture,
                                  is_last_unlocked_by_target_,
                                  privileged && allow_privileged_mouse_lock_);
    is_last_unlocked_by_target_ = false;
    return;
  }

  if (privileged && allow_privileged_mouse_lock_) {
    GotResponseToLockMouseRequest(true);
  } else {
    GotResponseToLockMouseRequest(false);
  }
}
