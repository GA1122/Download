  ~ScopedObserveWindowAnimation() {
    if (!Shell::Get()->tablet_mode_controller())
      return;

    if (!window_)
      return;

    if (window_->layer()->GetAnimator()->is_animating() &&
        (exiting_tablet_mode_ || manager_->IsTrackingWindow(window_))) {
      return;
    }

    Shell::Get()->tablet_mode_controller()->StopObservingAnimation(
         false,  true);
  }
