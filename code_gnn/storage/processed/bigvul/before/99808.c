void WebPluginDelegateStub::OnDidReceiveResponse(
    const PluginMsg_DidReceiveResponseParams& params) {
  WebPluginResourceClient* client = webplugin_->GetResourceClient(params.id);
  if (!client)
    return;

  client->DidReceiveResponse(params.mime_type,
                             params.headers,
                             params.expected_length,
                             params.last_modified,
                             params.request_is_seekable);
}
