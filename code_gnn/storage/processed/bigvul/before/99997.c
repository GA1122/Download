void WebPluginImpl::RemoveClient(size_t i) {
  clients_.erase(clients_.begin() + i);
}
