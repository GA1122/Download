void DevToolsWindow::OpenDevToolsWindow(
    content::WebContents* inspected_web_contents) {
  ToggleDevToolsWindow(
        inspected_web_contents, true, DevToolsToggleAction::Show(), "");
}
