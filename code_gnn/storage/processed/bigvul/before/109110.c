void RenderViewImpl::didReceiveResponse(
    WebFrame* frame, unsigned identifier, const WebURLResponse& response) {

  if (!frame->provisionalDataSource() || frame->parent())
    return;

  if (frame->isViewSourceModeEnabled())
    return;

  DocumentState* document_state =
      DocumentState::FromDataSource(frame->provisionalDataSource());
  int http_status_code = response.httpStatusCode();

  document_state->set_was_fetched_via_spdy(response.wasFetchedViaSPDY());
  document_state->set_was_npn_negotiated(response.wasNpnNegotiated());
  WebURLResponseExtraDataImpl* extra_data = GetExtraDataFromResponse(response);
  if (extra_data) {
    document_state->set_npn_negotiated_protocol(
        extra_data->npn_negotiated_protocol());
  }
  document_state->set_was_alternate_protocol_available(
      response.wasAlternateProtocolAvailable());
  document_state->set_was_fetched_via_proxy(response.wasFetchedViaProxy());
  document_state->set_http_status_code(http_status_code);
  document_state->set_use_error_page(true);
}
