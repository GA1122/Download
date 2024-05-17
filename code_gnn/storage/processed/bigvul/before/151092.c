void DevToolsWindow::OpenDevToolsWindow(
    content::WebContents* inspected_web_contents,
    const DevToolsToggleAction& action) {
  ToggleDevToolsWindow(inspected_web_contents, true, action, "");
}
