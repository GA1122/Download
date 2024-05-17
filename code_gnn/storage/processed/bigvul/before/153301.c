void DesktopWindowTreeHostX11::StackAbove(aura::Window* window) {
  if (window && window->GetRootWindow()) {
    ::Window window_below = window->GetHost()->GetAcceleratedWidget();
    std::vector<::Window> window_below_parents =
        GetParentsList(xdisplay_, window_below);
    std::vector<::Window> window_above_parents =
        GetParentsList(xdisplay_, xwindow_);

    auto it_below_window = window_below_parents.rbegin();
    auto it_above_window = window_above_parents.rbegin();
    for (; it_below_window != window_below_parents.rend() &&
           it_above_window != window_above_parents.rend() &&
           *it_below_window == *it_above_window;
         ++it_below_window, ++it_above_window) {
    }

    if (it_below_window != window_below_parents.rend() &&
        it_above_window != window_above_parents.rend()) {
      ::Window windows[] = {*it_below_window, *it_above_window};
      if (XRestackWindows(xdisplay_, windows, 2) == 0) {
        std::swap(windows[0], windows[1]);
        XRestackWindows(xdisplay_, windows, 2);
      }
    }
  }
}
