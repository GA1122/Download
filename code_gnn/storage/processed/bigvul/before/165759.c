  NetworkSessionConfiguratorTest()
      : quic_user_agent_id_("Chrome/52.0.2709.0 Linux x86_64") {
    field_trial_list_.reset(new base::FieldTrialList(
        std::make_unique<base::MockEntropyProvider>()));
    variations::testing::ClearAllVariationParams();
  }
