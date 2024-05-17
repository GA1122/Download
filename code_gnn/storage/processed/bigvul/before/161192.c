  static bool IsFlagExpired(const flags_ui::FeatureEntry& entry) {
    return flags::IsFlagExpired(entry.internal_name);
  }
