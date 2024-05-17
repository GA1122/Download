ExtensionDevToolsClientHost* DebuggerFunction::FindClientHost() {
  if (!agent_host_.get())
    return nullptr;

  const std::string& extension_id = extension()->id();
  DevToolsAgentHost* agent_host = agent_host_.get();
  AttachedClientHosts& hosts = g_attached_client_hosts.Get();
  auto it = std::find_if(
      hosts.begin(), hosts.end(),
      [&agent_host, &extension_id](ExtensionDevToolsClientHost* client_host) {
        return client_host->agent_host() == agent_host &&
               client_host->extension_id() == extension_id;
      });

  return it == hosts.end() ? nullptr : *it;
}
