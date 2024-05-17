  void SetUserLevelPrefValue(const std::string& app_id,
                             bool enabled_on_lock_screen) {
    chromeos::NoteTakingHelper* helper = chromeos::NoteTakingHelper::Get();
    ASSERT_TRUE(helper);

    helper->SetPreferredApp(browser()->profile(), app_id);
    helper->SetPreferredAppEnabledOnLockScreen(browser()->profile(),
                                               enabled_on_lock_screen);
  }
