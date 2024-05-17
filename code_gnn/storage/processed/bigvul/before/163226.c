void ExpectChildFrameCollapsedInLayout(Shell* shell,
                                       const std::string& frame_id,
                                       bool expect_collapsed) {
  const char kScript[] =
      "window.domAutomationController.send("
      "  document.getElementById(\"%s\").clientWidth"
      ");";
  int client_width = 0;
  EXPECT_TRUE(ExecuteScriptAndExtractInt(
      shell, base::StringPrintf(kScript, frame_id.c_str()), &client_width));
  EXPECT_EQ(expect_collapsed, !client_width) << client_width;
}
