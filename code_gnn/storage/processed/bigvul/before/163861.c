std::string WrapForJavascriptAndExtract(const char* javascript_expression) {
  return std::string("window.domAutomationController.send(") +
      javascript_expression + ")";
}
