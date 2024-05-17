void RenderViewHostImpl::RequestToLockMouse(bool user_gesture,
                                            bool last_unlocked_by_target) {
  delegate_->RequestToLockMouse(user_gesture, last_unlocked_by_target);
}
