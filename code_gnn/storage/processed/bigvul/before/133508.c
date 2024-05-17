bool ShouldStayInSameRootWindow(const aura::Window* window) {
  return window && (window->GetProperty(kStayInSameRootWindowKey) ||
                    ShouldStayInSameRootWindow(window->parent()));
}
