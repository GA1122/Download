  void Wait() {
    EXPECT_NE(expected_state_, lifecycle_unit_->GetState());
    run_loop_.Run();
    EXPECT_EQ(expected_state_, lifecycle_unit_->GetState());
  }
