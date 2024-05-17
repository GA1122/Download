  void SetHovered(bool is_mouse_hovered) {
    MoveMouse(0, is_mouse_hovered ? 10 : top_container_->height() + 100);
  }
