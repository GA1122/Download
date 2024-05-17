void RenderViewImpl::willSendRequest(WebFrame* frame,
                                     unsigned identifier,
                                     WebURLRequest& request,
                                     const WebURLResponse& redirect_response) {
  WebFrame* top_frame = frame->top();
  if (!top_frame)
    top_frame = frame;
  WebDataSource* provisional_data_source = top_frame->provisionalDataSource();
  WebDataSource* top_data_source = top_frame->dataSource();
  WebDataSource* data_source =
      provisional_data_source ? provisional_data_source : top_data_source;

  GURL request_url(request.url());
  GURL new_url;
  if (content::GetContentClient()->renderer()->WillSendRequest(
          frame, request_url, &new_url)) {
    request.setURL(WebURL(new_url));
  }

  content::PageTransition transition_type = content::PAGE_TRANSITION_LINK;
  DocumentState* document_state = DocumentState::FromDataSource(data_source);
  NavigationState* navigation_state = document_state->navigation_state();
  if (document_state) {
    if (document_state->is_cache_policy_override_set())
      request.setCachePolicy(document_state->cache_policy_override());
    transition_type = navigation_state->transition_type();
  }

  request.setExtraData(
      new RequestExtraData(frame->referrerPolicy(),
                           (frame == top_frame),
                           frame->identifier(),
                           frame->parent() == top_frame,
                           frame->parent() ? frame->parent()->identifier() : -1,
                           transition_type,
                           navigation_state->transferred_request_child_id(),
                           navigation_state->transferred_request_request_id()));

  DocumentState* top_document_state =
      DocumentState::FromDataSource(top_data_source);
  if (top_document_state &&
      (request.targetType() == WebURLRequest::TargetIsPrefetch ||
       request.targetType() == WebURLRequest::TargetIsPrerender))
    top_document_state->set_was_prefetcher(true);

  request.setRequestorID(routing_id_);
  request.setHasUserGesture(frame->isProcessingUserGesture());

  if (!renderer_preferences_.enable_referrers)
    request.clearHTTPHeaderField("Referer");
}
