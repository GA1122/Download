  void EndTestShouldRunAfterDidCommit() {
    EXPECT_TRUE(did_commit_);
    EndTest();
  }
