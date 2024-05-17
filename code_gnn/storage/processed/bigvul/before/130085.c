  ~TestVariationsPrefsStore() {
#if defined(OS_CHROMEOS)
    EXPECT_TRUE(
        cros_settings_->RemoveSettingsProvider(&stub_settings_provider_));
    cros_settings_->AddSettingsProvider(device_settings_provider_);
#endif
  }
