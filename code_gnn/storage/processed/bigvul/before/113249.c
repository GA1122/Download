void ShelfLayoutManager::UpdateAutoHideState() {
  AutoHideState auto_hide_state =
      CalculateAutoHideState(state_.visibility_state);
  if (auto_hide_state != state_.auto_hide_state) {
    if (auto_hide_state == AUTO_HIDE_HIDDEN) {
      SetState(state_.visibility_state);
      FOR_EACH_OBSERVER(Observer, observers_,
                        OnAutoHideStateChanged(auto_hide_state));
    } else {
      auto_hide_timer_.Stop();
      auto_hide_timer_.Start(
          FROM_HERE,
          base::TimeDelta::FromMilliseconds(kAutoHideDelayMS),
          this, &ShelfLayoutManager::UpdateAutoHideStateNow);
      FOR_EACH_OBSERVER(Observer, observers_, OnAutoHideStateChanged(
          CalculateAutoHideState(state_.visibility_state)));
    }
  } else {
    auto_hide_timer_.Stop();
  }
}
