void GamepadProvider::CheckForUserGesture() {
  base::AutoLock lock(user_gesture_lock_);
  if (user_gesture_observers_.empty() && ever_had_user_gesture_)
    return;

  const Gamepads* pads = gamepad_shared_buffer_->buffer();
  if (GamepadsHaveUserGesture(*pads)) {
    ever_had_user_gesture_ = true;
    for (size_t i = 0; i < user_gesture_observers_.size(); i++) {
      user_gesture_observers_[i].task_runner->PostTask(
          FROM_HERE, user_gesture_observers_[i].closure);
    }
    user_gesture_observers_.clear();
  }
}
