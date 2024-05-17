  virtual ~MockAudioRendererHost() {
    EXPECT_TRUE(delegates_.empty());
  }
