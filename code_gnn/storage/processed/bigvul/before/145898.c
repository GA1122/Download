  bool SelectWindow(const aura::Window* window) {
    if (GetSelectedWindow() == nullptr)
      SendKey(ui::VKEY_TAB);
    const aura::Window* start_window = GetSelectedWindow();
    if (start_window == window)
      return true;
    do {
      SendKey(ui::VKEY_TAB);
    } while (GetSelectedWindow() != window &&
             GetSelectedWindow() != start_window);
    return GetSelectedWindow() == window;
  }
