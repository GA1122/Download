void PostMessageAndWaitForReply(FrameTreeNode* sender_ftn,
                                const std::string& post_message_script,
                                const std::string& reply_status) {
  DOMMessageQueue msg_queue;

  bool success = false;
  EXPECT_TRUE(ExecuteScriptAndExtractBool(
      sender_ftn,
      "window.domAutomationController.send(" + post_message_script + ");",
      &success));
  EXPECT_TRUE(success);

  std::string status;
  while (msg_queue.WaitForMessage(&status)) {
    if (status == reply_status)
      break;
  }
}
