bool ShelfLayoutManager::ShouldBlurShelfBackground() {
  return IsBackgroundBlurEnabled() &&
         shelf_background_type_ == SHELF_BACKGROUND_DEFAULT &&
         state_.session_state == session_manager::SessionState::ACTIVE;
}
