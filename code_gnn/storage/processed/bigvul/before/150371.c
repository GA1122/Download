bool ClientControlledShellSurface::IsInputEnabled(Surface* surface) const {
  return surface == root_surface();
}
