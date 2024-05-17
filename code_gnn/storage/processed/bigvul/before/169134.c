std::string GetTextContent(content::RenderFrameHost* frame) {
  std::string result;
  EXPECT_TRUE(ExecuteScriptAndExtractString(
      frame, "domAutomationController.send(document.body.innerText)", &result));
  return result;
}
