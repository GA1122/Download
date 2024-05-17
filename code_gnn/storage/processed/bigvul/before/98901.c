  void TestProcessFrameData(WebSocket* websocket,
                            const char* expected_remaining_data,
                            int expected_remaining_len) {
    websocket->ProcessFrameData();

    const char* actual_remaining_data =
        websocket->current_read_buf_->StartOfBuffer()
        + websocket->read_consumed_len_;
    int actual_remaining_len =
        websocket->current_read_buf_->offset() - websocket->read_consumed_len_;

    EXPECT_EQ(expected_remaining_len, actual_remaining_len);
    EXPECT_TRUE(!memcmp(expected_remaining_data, actual_remaining_data,
                        expected_remaining_len));
  }
