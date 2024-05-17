  void RunCommandAndWaitForResponse(FrameTreeNode* node,
                                    const std::string& command,
                                    const std::string& response) {
    std::string msg_from_renderer;
    ASSERT_TRUE(
        ExecuteScriptAndExtractString(node, command, &msg_from_renderer));
    ASSERT_EQ(response, msg_from_renderer);
  }
