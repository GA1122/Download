  void VerifyCollectedEvent(size_t i,
                            unsigned phase,
                            const std::string& category,
                            const std::string& name) {
    EXPECT_EQ(phase, collected_events_phases_[i]);
    EXPECT_EQ(category, collected_events_categories_[i]);
    EXPECT_EQ(name, collected_events_names_[i]);
  }
