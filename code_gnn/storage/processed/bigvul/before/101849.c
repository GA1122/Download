void Browser::ToggleDevToolsWindow(DevToolsToggleAction action) {
  if (action == DEVTOOLS_TOGGLE_ACTION_SHOW_CONSOLE)
    UserMetrics::RecordAction(UserMetricsAction("DevTools_ToggleConsole"));
  else
    UserMetrics::RecordAction(UserMetricsAction("DevTools_ToggleWindow"));

  DevToolsWindow::ToggleDevToolsWindow(
      GetSelectedTabContentsWrapper()->render_view_host(), action);
}
