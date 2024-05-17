void DevToolsWindow::OpenDevToolsWindowForFrame(
    Profile* profile,
    const scoped_refptr<content::DevToolsAgentHost>& agent_host) {
  DevToolsWindow* window = FindDevToolsWindow(agent_host.get());
  if (!window) {
    window = DevToolsWindow::Create(profile, nullptr, kFrontendDefault,
                                    std::string(), false, std::string(),
                                    std::string());
    if (!window)
      return;
    window->bindings_->AttachTo(agent_host);
  }
  window->ScheduleShow(DevToolsToggleAction::Show());
}
