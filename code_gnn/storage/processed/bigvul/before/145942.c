  void DragStart(aura::Window* window) {
    resizer_ = CreateWindowResizer(window, window->bounds().origin(), HTCAPTION,
                                   ::wm::WINDOW_MOVE_SOURCE_MOUSE);
  }
