void SetExperimentEnabled(FlagsStorage* flags_storage,
                          const std::string& internal_name,
                          bool enable) {
  FlagsState::GetInstance()->SetExperimentEnabled(flags_storage,
                                                  internal_name, enable);
}
