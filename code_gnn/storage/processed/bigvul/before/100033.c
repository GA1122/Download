void WebPluginImpl::willSendRequest(WebURLLoader* loader,
                                    WebURLRequest& request,
                                    const WebURLResponse&) {
  WebPluginResourceClient* client = GetClientFromLoader(loader);
  if (client)
    client->WillSendRequest(request.url());
}
