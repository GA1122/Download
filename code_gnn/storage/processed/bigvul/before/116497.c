  ExtensionDevToolsClientHost* Lookup(WebContents* contents) {
    for (std::set<DevToolsClientHost*>::iterator it = client_hosts_.begin();
         it != client_hosts_.end(); ++it) {
      DevToolsAgentHost* agent_host =
          DevToolsManager::GetInstance()->GetDevToolsAgentHostFor(*it);
      if (!agent_host)
        continue;
      content::RenderViewHost* rvh =
          DevToolsAgentHostRegistry::GetRenderViewHost(agent_host);
      if (rvh && rvh->GetDelegate() &&
          rvh->GetDelegate()->GetAsWebContents() == contents)
        return static_cast<ExtensionDevToolsClientHost*>(*it);
    }
    return NULL;
  }
