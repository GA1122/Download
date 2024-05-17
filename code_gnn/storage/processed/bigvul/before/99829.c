WebPluginResourceClient* WebPluginProxy::GetResourceClient(int id) {
  ResourceClientMap::iterator iterator = resource_clients_.find(id);
  if (iterator == resource_clients_.end()) {
    return NULL;
  }

  return iterator->second;
}
