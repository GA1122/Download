void DevToolsWindow::ToggleDevToolsWindow(
    content::WebContents* inspected_web_contents,
    bool force_open,
    const DevToolsToggleAction& action,
    const std::string& settings) {
  scoped_refptr<DevToolsAgentHost> agent(
      DevToolsAgentHost::GetOrCreateFor(inspected_web_contents));
  DevToolsWindow* window = FindDevToolsWindow(agent.get());
  bool do_open = force_open;
  if (!window) {
    Profile* profile = Profile::FromBrowserContext(
        inspected_web_contents->GetBrowserContext());
    content::RecordAction(
        base::UserMetricsAction("DevTools_InspectRenderer"));
    window = Create(profile, GURL(), inspected_web_contents,
                    false, false, std::string(), true, settings);
    if (!window)
      return;
    window->bindings_->AttachTo(agent.get());
    do_open = true;
  }

  window->UpdateBrowserToolbar();

  if (!window->is_docked_ || do_open)
    window->ScheduleShow(action);
  else
    window->CloseWindow();
}
