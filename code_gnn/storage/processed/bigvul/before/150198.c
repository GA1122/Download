void TabletModeWindowManager::AddWindowCreationObservers() {
  DCHECK(observed_container_windows_.empty());
  for (aura::Window* root : Shell::GetAllRootWindows()) {
    for (auto* desk_container : desks_util::GetDesksContainers(root)) {
      DCHECK(!base::Contains(observed_container_windows_, desk_container));
      desk_container->AddObserver(this);
      observed_container_windows_.insert(desk_container);
    }
  }
}
