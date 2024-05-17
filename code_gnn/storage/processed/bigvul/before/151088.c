void DevToolsWindow::OnPageCloseCanceled(WebContents* contents) {
  DevToolsWindow* window =
      DevToolsWindow::GetInstanceForInspectedWebContents(contents);
  if (!window)
    return;
  window->intercepted_page_beforeunload_ = false;
  DevToolsWindow::OnPageCloseCanceled(window->main_web_contents_);
}
