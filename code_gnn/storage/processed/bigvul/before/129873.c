  void VerifyCallbackAndRecordedEvents(size_t expected_callback_count,
                                       size_t expected_recorded_count) {
    EXPECT_EQ(expected_callback_count, collected_events_names_.size());
    for (size_t i = 0; i < collected_events_names_.size(); ++i) {
      EXPECT_EQ("callback", collected_events_categories_[i]);
      EXPECT_EQ("yes", collected_events_names_[i]);
    }

    EXPECT_EQ(expected_recorded_count, trace_parsed_.GetSize());
    EXPECT_TRUE(FindTraceEntry(trace_parsed_, "recording"));
    EXPECT_FALSE(FindTraceEntry(trace_parsed_, "callback"));
    EXPECT_TRUE(FindTraceEntry(trace_parsed_, "yes"));
    EXPECT_FALSE(FindTraceEntry(trace_parsed_, "no"));
  }
