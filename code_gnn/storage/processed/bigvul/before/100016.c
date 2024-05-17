void WebPluginImpl::didFinishLoading(WebURLLoader* loader) {
  ClientInfo* client_info = GetClientInfoFromLoader(loader);
  if (client_info && client_info->client) {
    MultiPartResponseHandlerMap::iterator index =
      multi_part_response_map_.find(client_info->client);
    if (index != multi_part_response_map_.end()) {
      delete (*index).second;
      multi_part_response_map_.erase(index);
      if (page_delegate_)
        page_delegate_->DidStopLoadingForPlugin();
    }
    loader->setDefersLoading(true);
    WebPluginResourceClient* resource_client = client_info->client;
    client_info->client = NULL;
    resource_client->DidFinishLoading();

    if (WebDevToolsAgent* devtools_agent = GetDevToolsAgent())
      devtools_agent->didFinishLoading(client_info->id);
  }
}
