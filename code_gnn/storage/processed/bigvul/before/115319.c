  void MoveMouseAndWaitForExpansionStateChange(Panel* panel,
                                               const gfx::Point& position) {
    ui_test_utils::WindowedNotificationObserver signal(
        chrome::NOTIFICATION_PANEL_CHANGED_EXPANSION_STATE,
        content::Source<Panel>(panel));
    MoveMouse(position);
    signal.Wait();
  }
