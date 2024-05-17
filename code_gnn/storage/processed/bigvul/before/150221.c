  ScopedObserveWindowAnimation(aura::Window* window,
                               TabletModeWindowManager* manager,
                               bool exiting_tablet_mode)
      : window_(window),
        manager_(manager),
        exiting_tablet_mode_(exiting_tablet_mode) {
    if (Shell::Get()->tablet_mode_controller() && window_) {
      Shell::Get()->tablet_mode_controller()->MaybeObserveBoundsAnimation(
          window_);
    }
  }
