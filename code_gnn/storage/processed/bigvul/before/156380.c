ExtensionDevToolsClientHost::~ExtensionDevToolsClientHost() {
  if (infobar_)
    infobar_->Remove(this);
  g_attached_client_hosts.Get().erase(this);
}
