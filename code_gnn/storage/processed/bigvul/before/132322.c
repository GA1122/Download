void RenderFrameImpl::didStartProvisionalLoad(blink::WebLocalFrame* frame,
                                              double triggering_event_time) {
  DCHECK(!frame_ || frame_ == frame);
  WebDataSource* ds = frame->provisionalDataSource();

  if (!ds)
    return;

  TRACE_EVENT2("navigation", "RenderFrameImpl::didStartProvisionalLoad",
               "id", routing_id_, "url", ds->request().url().string().utf8());
  DocumentState* document_state = DocumentState::FromDataSource(ds);

  CHECK_IMPLIES(ds->request().url() == GURL(kSwappedOutURL), is_swapped_out_)
      << "Heard swappedout:// when not swapped out.";

  if (document_state->request_time().is_null() &&
          triggering_event_time != 0.0) {
    document_state->set_request_time(Time::FromDoubleT(triggering_event_time));
  }

  document_state->set_start_load_time(Time::Now());

  bool is_top_most = !frame->parent();
  if (is_top_most) {
    render_view_->set_navigation_gesture(
        WebUserGestureIndicator::isProcessingUserGesture() ?
            NavigationGestureUser : NavigationGestureAuto);
  } else if (ds->replacesCurrentHistoryItem()) {
    static_cast<NavigationStateImpl*>(document_state->navigation_state())
        ->set_transition_type(ui::PAGE_TRANSITION_AUTO_SUBFRAME);
  }

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidStartProvisionalLoad(frame));
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, DidStartProvisionalLoad());

  Send(new FrameHostMsg_DidStartProvisionalLoadForFrame(
       routing_id_, ds->request().url()));
}
