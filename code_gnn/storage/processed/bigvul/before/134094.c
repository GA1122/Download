  void OnTextInputStateChanged(const TextInputClient* client) {
    EXPECT_TRUE(call_expected_);

    EXPECT_EQ(next_client_, client);

    EXPECT_TRUE(on_will_change_focused_client_called_);
    EXPECT_TRUE(on_did_change_focused_client_called_);
    EXPECT_FALSE(on_text_input_state_changed_);

    on_text_input_state_changed_ = true;
 }
