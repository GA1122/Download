  FlagsStateSingleton()
      : flags_state_(std::make_unique<flags_ui::FlagsState>(
            kFeatureEntries,
            base::size(kFeatureEntries),
            base::Bind(&FlagsStateSingleton::IsFlagExpired))) {}
