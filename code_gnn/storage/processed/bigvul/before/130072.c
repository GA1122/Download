  void SetVariationsRestrictParameterPolicyValue(const std::string& value) {
#if defined(OS_CHROMEOS)
    cros_settings_->SetString(chromeos::kVariationsRestrictParameter, value);
#else
    prefs_.SetString(prefs::kVariationsRestrictParameter, value);
#endif
  }
