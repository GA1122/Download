void WebPluginDelegateStub::OnDidFail(int id) {
  WebPluginResourceClient* client = webplugin_->GetResourceClient(id);
  if (!client)
    return;

  client->DidFail();
 }
