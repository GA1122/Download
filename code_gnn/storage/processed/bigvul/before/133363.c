aura::Window* WindowTreeHostManager::GetRootWindowForDisplayId(int64_t id) {
  AshWindowTreeHost* host = GetAshWindowTreeHostForDisplayId(id);
  CHECK(host);
  return GetWindow(host);
}
