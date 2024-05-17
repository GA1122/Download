  void DestroyTabletModeWindowManager() {
    Shell::Get()->tablet_mode_controller()->SetEnabledForTest(false);
    EXPECT_FALSE(TabletModeControllerTestApi().tablet_mode_window_manager());
  }
