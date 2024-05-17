void VerifyPostMessageToOpener(content::RenderFrameHost* sender,
                               content::RenderFrameHost* opener) {
  EXPECT_TRUE(
      ExecuteScript(opener,
                    "window.addEventListener('message', function(event) {\n"
                    "  event.source.postMessage(event.data, '*');\n"
                    "});"));

  EXPECT_TRUE(
      ExecuteScript(sender,
                    "window.addEventListener('message', function(event) {\n"
                    "  window.domAutomationController.send(event.data);\n"
                    "});"));

  std::string result;
  EXPECT_TRUE(ExecuteScriptAndExtractString(
      sender, "opener.postMessage('foo', '*');", &result));
  EXPECT_EQ("foo", result);
}
