void ShelfLayoutManager::OnWindowActivated(ActivationReason reason,
                                           aura::Window* gained_active,
                                           aura::Window* lost_active) {
  UpdateAutoHideStateNow();
}
