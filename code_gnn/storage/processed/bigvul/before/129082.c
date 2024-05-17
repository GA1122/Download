void AttachedClientHosts::Remove(ExtensionDevToolsClientHost* client_host) {
  client_hosts_.erase(client_host);
}
