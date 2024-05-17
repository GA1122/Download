void DevToolsWindow::OpenDevToolsWindow(
    scoped_refptr<content::DevToolsAgentHost> agent_host,
    Profile* profile) {
  if (!profile)
    profile = Profile::FromBrowserContext(agent_host->GetBrowserContext());

  if (!profile)
    return;

  std::string type = agent_host->GetType();

  bool is_worker = type == DevToolsAgentHost::kTypeServiceWorker ||
                   type == DevToolsAgentHost::kTypeSharedWorker;

  if (!agent_host->GetFrontendURL().empty()) {
    FrontendType frontend_type = kFrontendRemote;
    if (is_worker) {
      frontend_type = kFrontendWorker;
    } else if (type == "node") {
      frontend_type = kFrontendV8;
    }
    DevToolsWindow::OpenExternalFrontend(profile, agent_host->GetFrontendURL(),
                                         agent_host, frontend_type);
    return;
  }

  if (is_worker) {
    DevToolsWindow::OpenDevToolsWindowForWorker(profile, agent_host);
    return;
  }

  if (type == content::DevToolsAgentHost::kTypeFrame) {
    DevToolsWindow::OpenDevToolsWindowForFrame(profile, agent_host);
    return;
  }

  content::WebContents* web_contents = agent_host->GetWebContents();
  if (web_contents)
    DevToolsWindow::OpenDevToolsWindow(web_contents);
}
