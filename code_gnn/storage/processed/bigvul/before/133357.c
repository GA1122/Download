aura::Window::Windows WindowTreeHostManager::GetAllRootWindows() {
  aura::Window::Windows windows;
  for (WindowTreeHostMap::const_iterator it = window_tree_hosts_.begin();
       it != window_tree_hosts_.end(); ++it) {
    DCHECK(it->second);
    if (GetRootWindowController(GetWindow(it->second)))
      windows.push_back(GetWindow(it->second));
  }
  return windows;
}
