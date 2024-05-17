void ExtensionDevToolsInfoBar::Remove(
    ExtensionDevToolsClientHost* client_host) {
  callbacks_.erase(client_host);
  if (callbacks_.empty())
    delete this;
}
