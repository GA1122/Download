  void AddFocusedInputField(FrameTreeNode* node) {
    ASSERT_TRUE(ExecuteScript(node, "addFocusedInputField();"));
  }
