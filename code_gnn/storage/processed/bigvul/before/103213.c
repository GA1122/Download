void Browser::ToggleDevToolsWindow(DevToolsToggleAction action) {
  std::string uma_string;
  switch (action) {
    case DEVTOOLS_TOGGLE_ACTION_SHOW_CONSOLE:
      uma_string = "DevTools_ToggleConsole";
      break;
    case DEVTOOLS_TOGGLE_ACTION_NONE:
    case DEVTOOLS_TOGGLE_ACTION_INSPECT:
    default:
      uma_string = "DevTools_ToggleWindow";
      break;
  }
  UserMetrics::RecordAction(UserMetricsAction(uma_string.c_str()), profile_);
  DevToolsManager::GetInstance()->ToggleDevToolsWindow(
      GetSelectedTabContentsWrapper()->render_view_host(), action);
}
