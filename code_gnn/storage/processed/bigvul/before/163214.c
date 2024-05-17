int RendererHistoryLength(Shell* shell) {
  int value = 0;
  EXPECT_TRUE(ExecuteScriptAndExtractInt(
      shell, "domAutomationController.send(history.length)", &value));
  return value;
}
