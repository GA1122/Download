  bool IsDragStarted() const {
    return drag_controller_ && drag_controller_->started_drag();
  }
