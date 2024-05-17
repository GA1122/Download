 void WebPluginDelegateStub::OnDidReceiveManualResponse(
    const GURL& url,
    const PluginMsg_DidReceiveResponseParams& params) {
  delegate_->DidReceiveManualResponse(url, params.mime_type, params.headers,
                                      params.expected_length,
                                      params.last_modified);
}
