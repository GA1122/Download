void LockScreenMediaControlsView::MediaSessionActionsChanged(
    const std::vector<MediaSessionAction>& actions) {
  if (hide_controls_timer_->IsRunning())
    return;

  enabled_actions_ =
      std::set<MediaSessionAction>(actions.begin(), actions.end());

  UpdateActionButtonsVisibility();
}
