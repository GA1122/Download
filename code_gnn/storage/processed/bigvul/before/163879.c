void DevToolsWindow::OpenExternalFrontend(
    Profile* profile,
    const std::string& frontend_url,
    const scoped_refptr<content::DevToolsAgentHost>& agent_host,
    FrontendType frontend_type) {
  DevToolsWindow* window = FindDevToolsWindow(agent_host.get());
  if (!window) {
    window = Create(profile, nullptr, frontend_type,
                    DevToolsUI::GetProxyURL(frontend_url).spec(), false,
                    std::string(), std::string());
    if (!window)
      return;
    window->bindings_->AttachTo(agent_host);
    window->close_on_detach_ = false;
  }

  window->ScheduleShow(DevToolsToggleAction::Show());
}
