void RenderViewImpl::didFailProvisionalLoad(WebFrame* frame,
                                            const WebURLError& error) {
  WebDataSource* ds = frame->provisionalDataSource();
  DCHECK(ds);

  const WebURLRequest& failed_request = ds->request();

  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, DidFailProvisionalLoad(frame, error));

  bool show_repost_interstitial =
      (error.reason == net::ERR_CACHE_MISS &&
       EqualsASCII(failed_request.httpMethod(), "POST"));

  ViewHostMsg_DidFailProvisionalLoadWithError_Params params;
  params.frame_id = frame->identifier();
  params.is_main_frame = !frame->parent();
  params.error_code = error.reason;
  GetContentClient()->renderer()->GetNavigationErrorStrings(
      failed_request,
      error,
      NULL,
      &params.error_description);
  params.url = error.unreachableURL;
  params.showing_repost_interstitial = show_repost_interstitial;
  Send(new ViewHostMsg_DidFailProvisionalLoadWithError(
      routing_id_, params));

  if (error.reason == net::ERR_ABORTED)
    return;

  frame->enableViewSourceMode(false);

  DocumentState* document_state = DocumentState::FromDataSource(ds);
  NavigationState* navigation_state = document_state->navigation_state();

  bool replace =
      navigation_state->pending_page_id() != -1 ||
      navigation_state->transition_type() ==
          PAGE_TRANSITION_AUTO_SUBFRAME;

  if (!navigation_state->is_content_initiated()) {
    pending_navigation_params_.reset(new ViewMsg_Navigate_Params);
    pending_navigation_params_->page_id =
        navigation_state->pending_page_id();
    pending_navigation_params_->pending_history_list_offset =
        navigation_state->pending_history_list_offset();
    pending_navigation_params_->transition =
        navigation_state->transition_type();
    pending_navigation_params_->request_time =
        document_state->request_time();
  }

  if (MaybeLoadAlternateErrorPage(frame, error, replace))
    return;

  LoadNavigationErrorPage(frame, failed_request, error, std::string(), replace);
}
