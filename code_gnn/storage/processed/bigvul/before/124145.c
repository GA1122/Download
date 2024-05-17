  virtual ~TestDevToolsClientHost() {
    EXPECT_TRUE(closed_);
  }
