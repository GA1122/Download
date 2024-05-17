void WebPluginImpl::SetDeferResourceLoading(unsigned long resource_id,
                                            bool defer) {
  std::vector<ClientInfo>::iterator client_index = clients_.begin();
  while (client_index != clients_.end()) {
    ClientInfo& client_info = *client_index;

    if (client_info.id == resource_id) {
      client_info.loader->setDefersLoading(defer);

      if (!defer && client_info.client &&
          client_info.pending_failure_notification) {
        WebPluginResourceClient* resource_client = client_info.client;
        client_info.loader->cancel();
        clients_.erase(client_index++);
        resource_client->DidFail();

        if (WebDevToolsAgent* devtools_agent = GetDevToolsAgent())
          devtools_agent->didFinishLoading(resource_id);
      }
      break;
    }
    client_index++;
  }
}
