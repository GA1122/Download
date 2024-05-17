  void AssertBadMsgReported() {
    EXPECT_EQ(render_process_host_.bad_msg_count() + host_->bad_msg_count, 1);
  }
