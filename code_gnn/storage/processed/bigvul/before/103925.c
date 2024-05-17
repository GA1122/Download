void RenderView::willSendRequest(
    WebFrame* frame, unsigned identifier, WebURLRequest& request,
    const WebURLResponse& redirect_response) {
  WebFrame* top_frame = frame->top();
  if (!top_frame)
    top_frame = frame;
  WebDataSource* provisional_data_source = top_frame->provisionalDataSource();
  WebDataSource* top_data_source = top_frame->dataSource();
  WebDataSource* data_source =
      provisional_data_source ? provisional_data_source : top_data_source;

  bool is_top_frame = (frame == top_frame);
  request.setExtraData(
      new RequestExtraData(is_top_frame, frame->identifier()));

  GURL request_url(request.url());
  GURL new_url;
  if (content::GetContentClient()->renderer()->WillSendRequest(
          frame, request_url, &new_url)) {
    request.setURL(WebURL(new_url));
  }

  if (data_source) {
    NavigationState* state = NavigationState::FromDataSource(data_source);
    if (state && state->is_cache_policy_override_set())
      request.setCachePolicy(state->cache_policy_override());
  }

  if (top_data_source) {
    NavigationState* state = NavigationState::FromDataSource(top_data_source);
    if (state && (request.targetType() == WebURLRequest::TargetIsPrefetch ||
                  request.targetType() == WebURLRequest::TargetIsPrerender))
      state->set_was_prefetcher(true);
  }

  request.setRequestorID(routing_id_);
  request.setHasUserGesture(frame->isProcessingUserGesture());

  if (!renderer_preferences_.enable_referrers)
    request.clearHTTPHeaderField("Referer");

  SiteIsolationMetrics::AddRequest(identifier, request.targetType());
}
