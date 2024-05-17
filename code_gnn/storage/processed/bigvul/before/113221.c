bool ShelfLayoutManager::GetLauncherPaintsBackground() const {
  return (!state_.is_screen_locked && window_overlaps_shelf_) ||
      state_.visibility_state == AUTO_HIDE;
}
