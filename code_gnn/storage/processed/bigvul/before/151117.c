void DevToolsWindow::ToggleDevToolsWindow(
    Browser* browser,
    const DevToolsToggleAction& action) {
  if (action.type() == DevToolsToggleAction::kToggle &&
      browser->is_devtools()) {
    browser->tab_strip_model()->CloseAllTabs();
    return;
  }

  ToggleDevToolsWindow(
      browser->tab_strip_model()->GetActiveWebContents(),
      action.type() == DevToolsToggleAction::kInspect,
      action, "");
}
