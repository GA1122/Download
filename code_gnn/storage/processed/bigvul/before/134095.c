  void OnWillChangeFocusedClient(TextInputClient* focused_before,
                                 TextInputClient* focused) {
    EXPECT_TRUE(call_expected_);

    EXPECT_EQ(previous_client_, focused_before);
    EXPECT_EQ(next_client_, focused);

    EXPECT_FALSE(on_will_change_focused_client_called_);
    EXPECT_FALSE(on_did_change_focused_client_called_);
    EXPECT_FALSE(on_text_input_state_changed_);

    on_will_change_focused_client_called_ = true;
  }
