  void WaitForOnLoad(FrameTreeNode* node) {
    RunCommandAndWaitForResponse(node, "notifyWhenLoaded();", "LOADED");
  }
