void ShellSurface::OnWindowDestroying(aura::Window* window) {
  if (window == parent_) {
    parent_ = nullptr;
    SetEnabled(false);
  }
  window->RemoveObserver(this);
}
