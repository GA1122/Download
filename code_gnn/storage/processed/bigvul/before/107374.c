bool FocusedOnPage(TabContents* tab_contents, std::string* result) {
  return ui_test_utils::ExecuteJavaScriptAndExtractString(
      tab_contents->render_view_host(),
      L"",
      L"window.domAutomationController.send(getFocusedElement());",
      result);
}
