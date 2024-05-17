void DevToolsWindow::OpenNodeFrontendWindow(Profile* profile) {
  for (DevToolsWindow* window : g_instances.Get()) {
    if (window->frontend_type_ == kFrontendNode) {
      window->ActivateWindow();
      return;
    }
  }

  DevToolsWindow* window =
      Create(profile, nullptr, kFrontendNode, std::string(), false,
             std::string(), std::string());
  if (!window)
    return;
  window->bindings_->AttachTo(DevToolsAgentHost::CreateForDiscovery());
  window->ScheduleShow(DevToolsToggleAction::Show());
}
