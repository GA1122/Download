bool ShellSurface::CanResize() const {
  return initial_bounds_.IsEmpty();
}
