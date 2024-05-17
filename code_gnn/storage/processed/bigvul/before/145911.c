  OverviewWindowDragController* window_drag_controller() {
    DCHECK(window_selector());
    return window_selector()->window_drag_controller_.get();
  }
