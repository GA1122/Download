void ShelfLayoutManager::OnHomeLauncherTargetPositionChanged(
    bool showing,
    int64_t display_id) {
  if (!shelf_widget_ || !shelf_widget_->GetNativeWindow())
    return;

  if (display_.id() != display_id)
    return;

  is_home_launcher_target_position_shown_ = showing;
  MaybeUpdateShelfBackground(AnimationChangeType::IMMEDIATE);
}