ExtensionDevToolsClientHost* AttachedClientHosts::Lookup(
    DevToolsAgentHost* agent_host,
    const std::string& extension_id) {
  DevToolsManager* manager = DevToolsManager::GetInstance();
  for (ClientHosts::iterator it = client_hosts_.begin();
       it != client_hosts_.end(); ++it) {
    ExtensionDevToolsClientHost* client_host = *it;
    if (manager->GetDevToolsAgentHostFor(client_host) == agent_host &&
        client_host->extension_id() == extension_id)
      return client_host;
  }
  return NULL;
}
