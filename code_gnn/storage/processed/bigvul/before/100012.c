void WebPluginImpl::didFail(WebURLLoader* loader,
                            const WebURLError& error) {
  ClientInfo* client_info = GetClientInfoFromLoader(loader);
  if (client_info && client_info->client) {
    loader->setDefersLoading(true);
    WebPluginResourceClient* resource_client = client_info->client;
    client_info->client = NULL;
    resource_client->DidFail();

    if (WebDevToolsAgent* devtools_agent = GetDevToolsAgent())
      devtools_agent->didFailLoading(client_info->id, error);
  }
}
