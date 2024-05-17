void NavigationController::SetActive(bool is_active) {
  if (is_active && needs_reload_)
    LoadIfNecessary();
}
