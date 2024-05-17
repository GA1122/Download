void ShelfLayoutManager::SetAutoHideBehavior(ShelfAutoHideBehavior behavior) {
  if (auto_hide_behavior_ == behavior)
    return;
  auto_hide_behavior_ = behavior;
  UpdateVisibilityState();
  FOR_EACH_OBSERVER(Observer, observers_,
                    OnAutoHideStateChanged(state_.auto_hide_state));
}
