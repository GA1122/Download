void WebPluginDelegateStub::OnWillSendRequest(int id, const GURL& url) {
  WebPluginResourceClient* client = webplugin_->GetResourceClient(id);
  if (!client)
    return;

  client->WillSendRequest(url);
}
