  void SetWindowScroll(FrameTreeNode* node, int x, int y) {
    ASSERT_TRUE(ExecuteScript(
        node, base::StringPrintf("window.scrollTo(%d, %d);", x, y)));
  }
