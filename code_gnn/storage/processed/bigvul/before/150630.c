  void InvokeVisualStateCallback(bool result) {
    EXPECT_TRUE(result);
    visual_state_callback_count_++;
  }
