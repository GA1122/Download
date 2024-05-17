void WebPluginDelegateStub::OnDidFinishLoading(int id) {
  WebPluginResourceClient* client = webplugin_->GetResourceClient(id);
  if (!client)
    return;

  client->DidFinishLoading();
}
