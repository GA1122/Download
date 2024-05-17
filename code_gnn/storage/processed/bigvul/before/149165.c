void LockScreenMediaControlsView::EndDrag() {
  is_in_drag_ = false;

  if (last_fling_velocity_ <= kDragVelocityThreshold ||
      (contents_view_->GetBoundsInScreen().y() <= kHeightDismissalThreshold &&
       last_fling_velocity_ < 0)) {
    RunHideControlsAnimation();
    return;
  }

  RunResetControlsAnimation();
}
