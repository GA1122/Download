void PostMessageAndWaitForReply(FrameTreeNode* sender_ftn,
                                const std::string& post_message_script,
                                const std::string& reply_status) {
  DOMMessageQueue msg_queue;

  EXPECT_EQ(true, EvalJs(sender_ftn, "(" + post_message_script + ");"));

  std::string status;
  while (msg_queue.WaitForMessage(&status)) {
    if (status == reply_status)
      break;
  }
}
