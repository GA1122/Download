void ShelfLayoutManager::UpdateTargetBoundsForGesture(
    TargetBounds* target_bounds) const {
  CHECK_EQ(GESTURE_DRAG_IN_PROGRESS, gesture_drag_status_);
  const bool horizontal = shelf_->IsHorizontalAlignment();
  const int shelf_size = ShelfConstants::shelf_size();
  gfx::Rect available_bounds =
      screen_util::GetDisplayBoundsWithShelf(shelf_widget_->GetNativeWindow());
  int resistance_free_region = 0;
  bool hidden_at_start = false;

  if (gesture_drag_auto_hide_state_ == SHELF_AUTO_HIDE_HIDDEN &&
      visibility_state() == SHELF_AUTO_HIDE &&
      auto_hide_state() != SHELF_AUTO_HIDE_SHOWN) {
    resistance_free_region = shelf_size - kHiddenShelfInScreenPortion;
    hidden_at_start = true;
  }

  bool resist = SelectValueForShelfAlignment(
      gesture_drag_amount_<-resistance_free_region, gesture_drag_amount_>
          resistance_free_region,
      gesture_drag_amount_ < -resistance_free_region);

  float translate = 0.f;
  if (resist) {
    float diff = fabsf(gesture_drag_amount_) - resistance_free_region;
    diff = std::min(diff, sqrtf(diff));
    if (gesture_drag_amount_ < 0)
      translate = -resistance_free_region - diff;
    else
      translate = resistance_free_region + diff;
  } else {
    translate = gesture_drag_amount_;
  }
  const int baseline = SelectValueForShelfAlignment(
      available_bounds.bottom() - (hidden_at_start ? 0 : shelf_size),
      available_bounds.x() - (hidden_at_start ? shelf_size : 0),
      available_bounds.right() - (hidden_at_start ? 0 : shelf_size));
  if (horizontal) {
    target_bounds->shelf_bounds.set_y(baseline + translate);
    target_bounds->status_bounds_in_shelf.set_y(0);
  } else {
    target_bounds->shelf_bounds.set_x(baseline + translate);
    target_bounds->status_bounds_in_shelf.set_x(0);
  }
}
