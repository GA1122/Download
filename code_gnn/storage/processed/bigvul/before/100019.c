void WebPluginImpl::didReceiveData(WebURLLoader* loader,
                                   const char *buffer,
                                   int length) {
  WebPluginResourceClient* client = GetClientFromLoader(loader);
  if (!client)
    return;

  if (WebDevToolsAgent* devtools_agent = GetDevToolsAgent()) {
    ClientInfo* client_info = GetClientInfoFromLoader(loader);
    if (client_info)
      devtools_agent->didReceiveData(client_info->id, length);
  }
  MultiPartResponseHandlerMap::iterator index =
      multi_part_response_map_.find(client);
  if (index != multi_part_response_map_.end()) {
    MultipartResponseDelegate* multi_part_handler = (*index).second;
    DCHECK(multi_part_handler != NULL);
    multi_part_handler->OnReceivedData(buffer, length);
  } else {
    loader->setDefersLoading(true);
    client->DidReceiveData(buffer, length, 0);
  }
}
