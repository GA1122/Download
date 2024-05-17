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
    base::RecordAction(base::UserMetricsAction("DevTools_InspectRenderer"));
    std::string panel = "";
    switch (action.type()) {
      case DevToolsToggleAction::kInspect:
      case DevToolsToggleAction::kShowElementsPanel:
        panel = "elements";
        break;
      case DevToolsToggleAction::kShowConsolePanel:
        panel = "console";
        break;
      case DevToolsToggleAction::kShow:
      case DevToolsToggleAction::kToggle:
      case DevToolsToggleAction::kReveal:
      case DevToolsToggleAction::kNoOp:
        break;
    }
    window = Create(profile, inspected_web_contents, kFrontendDefault,
                    std::string(), true, settings, panel);
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
