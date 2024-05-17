  void WaitForViewportToStabilize(FrameTreeNode* node) {
    RunCommandAndWaitForResponse(node, "notifyWhenViewportStable(0);",
                                 "VIEWPORT_STABLE");
  }
