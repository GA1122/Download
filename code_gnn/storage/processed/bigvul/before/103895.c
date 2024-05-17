void RenderView::didReceiveResponse(
    WebFrame* frame, unsigned identifier, const WebURLResponse& response) {

  SiteIsolationMetrics::LogMimeTypeForCrossOriginRequest(frame,
                                                         identifier,
                                                         response);

  if (!frame->provisionalDataSource() || frame->parent())
    return;

  if (frame->isViewSourceModeEnabled())
    return;

  NavigationState* navigation_state =
      NavigationState::FromDataSource(frame->provisionalDataSource());
  CHECK(navigation_state);
  int http_status_code = response.httpStatusCode();

  navigation_state->set_was_fetched_via_spdy(response.wasFetchedViaSPDY());
  navigation_state->set_was_npn_negotiated(response.wasNpnNegotiated());
  navigation_state->set_was_alternate_protocol_available(
      response.wasAlternateProtocolAvailable());
  navigation_state->set_was_fetched_via_proxy(response.wasFetchedViaProxy());
  navigation_state->set_http_status_code(http_status_code);
  navigation_state->set_use_error_page(true);
}
