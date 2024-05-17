  void ExpectAndResetCounts(
      int expect_continues,
      int expect_cancels,
      int expect_asks,
      int line) {
    EXPECT_EQ(expect_continues, continue_count_) << "line " << line;
    EXPECT_EQ(expect_cancels, cancel_count_) << "line " << line;
    EXPECT_EQ(expect_asks, AskAllowCount()) << "line " << line;
    continue_count_ = cancel_count_ = 0;
    mock_permission_prompt_factory_->ResetCounts();
  }
