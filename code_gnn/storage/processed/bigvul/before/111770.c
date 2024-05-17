void SetExperimentEnabled(
    PrefService* prefs, const std::string& internal_name, bool enable) {
  FlagsState::GetInstance()->SetExperimentEnabled(prefs, internal_name, enable);
}
