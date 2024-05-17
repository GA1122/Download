void RenderFrameImpl::didFailProvisionalLoad(
    blink::WebLocalFrame* frame,
    const blink::WebURLError& error,
    blink::WebHistoryCommitType commit_type) {
  TRACE_EVENT1("navigation", "RenderFrameImpl::didFailProvisionalLoad",
               "id", routing_id_);
  DCHECK(!frame_ || frame_ == frame);
  WebDataSource* ds = frame->provisionalDataSource();
  DCHECK(ds);

  const WebURLRequest& failed_request = ds->request();

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidFailProvisionalLoad(frame, error));
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_,
                    DidFailProvisionalLoad(error));

  SendFailedProvisionalLoad(failed_request, error, frame);

  if (!ShouldDisplayErrorPageForFailedLoad(error.reason, error.unreachableURL))
    return;

  frame->enableViewSourceMode(false);

  DocumentState* document_state = DocumentState::FromDataSource(ds);
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());

  bool replace = commit_type != blink::WebStandardCommit;

  if (!navigation_state->IsContentInitiated()) {
    pending_navigation_params_.reset(new NavigationParams(
        navigation_state->common_params(), navigation_state->start_params(),
        navigation_state->request_params()));
    pending_navigation_params_->request_params.request_time =
        document_state->request_time();
  }

  LoadNavigationErrorPage(failed_request, error, replace);
}
