void WebPluginDelegateStub::OnDidReceiveData(int id,
                                             const std::vector<char>& buffer,
                                             int data_offset) {
  WebPluginResourceClient* client = webplugin_->GetResourceClient(id);
  if (!client)
    return;

  client->DidReceiveData(&buffer.front(), static_cast<int>(buffer.size()),
                         data_offset);
}
