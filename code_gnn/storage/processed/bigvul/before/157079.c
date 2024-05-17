  void Start() {
    EXPECT_TRUE(data_provider());
    EXPECT_FALSE(active_loader_allownull());
    data_provider()->Start();
  }
