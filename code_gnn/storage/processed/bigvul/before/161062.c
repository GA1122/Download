std::string DevToolsHttpHandler::GetFrontendURLInternal(
    scoped_refptr<DevToolsAgentHost> agent_host,
    const std::string& id,
    const std::string& host) {
  std::string frontend_url;
  if (delegate_->HasBundledFrontendResources()) {
    frontend_url = "/devtools/inspector.html";
  } else {
    std::string type = agent_host->GetType();
    bool is_worker = type == DevToolsAgentHost::kTypeServiceWorker ||
                     type == DevToolsAgentHost::kTypeSharedWorker;
    frontend_url = base::StringPrintf(
        "http://chrome-devtools-frontend.appspot.com/serve_rev/%s/%s.html",
        GetWebKitRevision().c_str(), is_worker ? "worker_app" : "inspector");
  }
  return base::StringPrintf("%s?ws=%s%s%s", frontend_url.c_str(), host.c_str(),
                            kPageUrlPrefix, id.c_str());
}
