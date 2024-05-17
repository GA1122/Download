  mojom::WindowStateType GetLastOldStateAndReset() {
    mojom::WindowStateType r = last_old_state_;
    last_old_state_ = mojom::WindowStateType::DEFAULT;
    return r;
  }
