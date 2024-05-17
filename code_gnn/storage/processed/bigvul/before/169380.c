bool WaitForRenderFrameReady(RenderFrameHost* rfh) {
  if (!rfh)
    return false;
  std::string result;
  EXPECT_TRUE(
      content::ExecuteScriptAndExtractString(
          rfh,
          "(function() {"
          "  var done = false;"
          "  function checkState() {"
          "    if (!done && document.readyState == 'complete') {"
          "      done = true;"
          "      window.domAutomationController.send('pageLoadComplete');"
          "    }"
          "  }"
          "  checkState();"
          "  document.addEventListener('readystatechange', checkState);"
          "})();",
          &result));
  return result == "pageLoadComplete";
}
