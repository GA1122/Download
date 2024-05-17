aura::Window* WindowTreeHostManager::GetPrimaryRootWindow() {
  return GetRootWindowForDisplayId(primary_display_id);
}
