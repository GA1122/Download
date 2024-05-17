  void Verify() {
    EXPECT_EQ(call_expected_, on_will_change_focused_client_called_);
    EXPECT_EQ(call_expected_, on_did_change_focused_client_called_);
    EXPECT_EQ(call_expected_, on_text_input_state_changed_);
  }
