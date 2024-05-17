  void OnCanDownloadDecided(bool allow) {
    if (!completion_closure_.is_null()) {
      base::ResetAndReturn(&completion_closure_).Run();
      EXPECT_EQ(allow, expectation_);
    } else {
      on_decided_called_ = true;
      last_allow_ = allow;
    }
  }
