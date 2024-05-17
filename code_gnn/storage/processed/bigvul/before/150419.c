  static ash::WindowResizer* CreateDragWindowResizer(
      aura::Window* window,
      const gfx::Point& point_in_parent,
      int window_component) {
    return ash::CreateWindowResizer(window, point_in_parent, window_component,
                                    ::wm::WINDOW_MOVE_SOURCE_MOUSE)
        .release();
  }
