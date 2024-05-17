void ShelfLayoutManager::OnWindowActivated(aura::Window* active,
                                           aura::Window* old_active) {
  UpdateAutoHideStateNow();
}
