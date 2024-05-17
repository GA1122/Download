void WebPluginImpl::TearDownPluginInstance(
    WebURLLoader* loader_to_ignore) {
  if (container_)
    container_->clearScriptObjects();

  if (delegate_) {
    delegate_->PluginDestroyed();
    delegate_ = NULL;
  }

  std::vector<ClientInfo>::iterator client_index = clients_.begin();
  while (client_index != clients_.end()) {
    ClientInfo& client_info = *client_index;

    if (loader_to_ignore == client_info.loader) {
      client_index++;
      continue;
    }

    if (client_info.loader.get())
      client_info.loader->cancel();

    client_index = clients_.erase(client_index);
  }

  webframe_ = NULL;
  method_factory_.RevokeAll();
}
