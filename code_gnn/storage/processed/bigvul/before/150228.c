  TabletModeWindowManager* CreateTabletModeWindowManager() {
    EXPECT_FALSE(TabletModeControllerTestApi().tablet_mode_window_manager());
    Shell::Get()->tablet_mode_controller()->SetEnabledForTest(true);
    return TabletModeControllerTestApi().tablet_mode_window_manager();
  }
