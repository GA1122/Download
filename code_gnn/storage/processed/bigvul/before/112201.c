aura::Window* StackingController::GetSystemModalContainer(
    aura::Window* window) const {
  if (!IsSystemModal(window))
    return NULL;

  aura::Window* lock_container =
      GetContainerById(internal::kShellWindowId_LockScreenContainer);
  if (!lock_container->children().size())
    return GetContainerById(internal::kShellWindowId_SystemModalContainer);

  int lock_container_id = lock_container->id();
  int window_container_id = window->transient_parent()->parent()->id();

  aura::Window* container = NULL;
  if (window_container_id < lock_container_id) {
    container = GetContainerById(
        internal::kShellWindowId_SystemModalContainer);
  } else {
    container = GetContainerById(
        internal::kShellWindowId_LockSystemModalContainer);
  }

  return container;
}
