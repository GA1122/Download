void RenderView::didStartProvisionalLoad(WebFrame* frame) {
  WebDataSource* ds = frame->provisionalDataSource();
  NavigationState* navigation_state = NavigationState::FromDataSource(ds);

  if (navigation_state->request_time().is_null()) {
    double event_time = ds->triggeringEventTime();
    if (event_time != 0.0)
      navigation_state->set_request_time(Time::FromDoubleT(event_time));
  }

  navigation_state->set_start_load_time(Time::Now());

  bool is_top_most = !frame->parent();
  if (is_top_most) {
    navigation_gesture_ = frame->isProcessingUserGesture() ?
        NavigationGestureUser : NavigationGestureAuto;

    completed_client_redirect_src_ = GURL();
  } else if (frame->parent()->isLoading()) {
    navigation_state->set_transition_type(PageTransition::AUTO_SUBFRAME);
  }

  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, DidStartProvisionalLoad(frame));

  bool has_opener_set = opener_id_ != MSG_ROUTING_NONE;
  Send(new ViewHostMsg_DidStartProvisionalLoadForFrame(
       routing_id_, frame->identifier(), is_top_most, has_opener_set,
       ds->request().url()));
}
