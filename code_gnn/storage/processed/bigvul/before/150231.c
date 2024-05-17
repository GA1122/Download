  WindowStateType GetLastOldStateAndReset() {
    WindowStateType r = last_old_state_;
    last_old_state_ = WindowStateType::kDefault;
    return r;
  }
