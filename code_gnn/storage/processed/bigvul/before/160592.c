void RenderFrameImpl::DidReceiveResponse(
    const blink::WebURLResponse& response) {
  if (!frame_->GetProvisionalDocumentLoader() || frame_->Parent())
    return;

  if (frame_->IsViewSourceModeEnabled())
    return;

  DocumentState* document_state =
      DocumentState::FromDocumentLoader(frame_->GetProvisionalDocumentLoader());
  int http_status_code = response.HttpStatusCode();

  WebURLResponseExtraDataImpl* extra_data = GetExtraDataFromResponse(response);
  if (extra_data) {
    document_state->set_was_fetched_via_spdy(
        extra_data->was_fetched_via_spdy());
    document_state->set_was_alpn_negotiated(extra_data->was_alpn_negotiated());
    document_state->set_alpn_negotiated_protocol(
        response.AlpnNegotiatedProtocol().Utf8());
    document_state->set_was_alternate_protocol_available(
        extra_data->was_alternate_protocol_available());
    document_state->set_connection_info(response.ConnectionInfo());
  }
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);
  internal_data->set_http_status_code(http_status_code);
}
