void ShelfLayoutManager::UpdateAutoHideState() {
  ShelfAutoHideState auto_hide_state =
      CalculateAutoHideState(state_.visibility_state);
  if (auto_hide_state != state_.auto_hide_state) {
    if (auto_hide_state == SHELF_AUTO_HIDE_HIDDEN) {
      SetState(state_.visibility_state);
    } else {
      if (!auto_hide_timer_.IsRunning()) {
        mouse_over_shelf_when_auto_hide_timer_started_ =
            shelf_widget_->GetWindowBoundsInScreen().Contains(
                display::Screen::GetScreen()->GetCursorScreenPoint());
      }
      auto_hide_timer_.Start(
          FROM_HERE, base::TimeDelta::FromMilliseconds(kAutoHideDelayMS), this,
          &ShelfLayoutManager::UpdateAutoHideStateNow);
    }
  } else {
    StopAutoHideTimer();
  }
}
