  TabletModeWindowManager* tablet_mode_window_manager() {
    return Shell::Get()
        ->tablet_mode_controller()
        ->tablet_mode_window_manager_.get();
  }
