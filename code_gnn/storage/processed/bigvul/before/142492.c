void ShelfLayoutManager::UpdateAutoHideStateNow() {
  SetState(state_.visibility_state);

  StopAutoHideTimer();
}
