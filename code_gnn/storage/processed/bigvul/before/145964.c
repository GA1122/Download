  TabletModeWindowManager* CreateTabletModeWindowManager() {
    EXPECT_FALSE(tablet_mode_window_manager());
    Shell::Get()->tablet_mode_controller()->EnableTabletModeWindowManager(true);
    return tablet_mode_window_manager();
  }
