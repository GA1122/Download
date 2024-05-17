std::vector<aura::Window*> Shell::GetContainersFromAllRootWindows(
    int container_id,
    aura::Window* priority_root) {
  std::vector<aura::Window*> containers;
  aura::Window::Windows root_windows = GetAllRootWindows();
  for (aura::Window::Windows::const_iterator it = root_windows.begin();
       it != root_windows.end(); ++it) {
    aura::Window* container = (*it)->GetChildById(container_id);
    if (container) {
      if (priority_root && priority_root->Contains(container))
        containers.insert(containers.begin(), container);
      else
        containers.push_back(container);
    }
  }
  return containers;
}
