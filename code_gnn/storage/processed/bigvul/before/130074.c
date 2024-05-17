  TestVariationsPrefsStore() {
#if defined(OS_ANDROID)
    VariationsService::RegisterProfilePrefs(prefs_.registry());
#else
    VariationsService::RegisterPrefs(prefs_.registry());
#endif

#if defined(OS_CHROMEOS)
    cros_settings_ = chromeos::CrosSettings::Get();
    DCHECK(cros_settings_ != NULL);
    device_settings_provider_ = cros_settings_->GetProvider(
        chromeos::kReportDeviceVersionInfo);
    EXPECT_TRUE(device_settings_provider_ != NULL);
    EXPECT_TRUE(cros_settings_->RemoveSettingsProvider(
        device_settings_provider_));
    cros_settings_->AddSettingsProvider(&stub_settings_provider_);
#endif
  }
