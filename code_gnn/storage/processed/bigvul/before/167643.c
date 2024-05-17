std::string GetDocumentOrigin(FrameTreeNode* ftn) {
  std::string origin;
  EXPECT_TRUE(ExecuteScriptAndExtractString(
      ftn, "domAutomationController.send(document.origin)", &origin));
  return origin;
}
