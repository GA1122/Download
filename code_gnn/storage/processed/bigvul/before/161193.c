  void RebuildState(const std::vector<flags_ui::FeatureEntry>& entries) {
    flags_state_ = std::make_unique<flags_ui::FlagsState>(
        entries.data(), entries.size(),
        base::Bind(&FlagsStateSingleton::IsFlagExpired));
  }
