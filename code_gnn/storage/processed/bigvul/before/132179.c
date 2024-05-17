void RenderFrameImpl::BeginNavigation(blink::WebURLRequest* request) {
  CHECK(base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableBrowserSideNavigation));
  DCHECK(request);

  willSendRequest(frame_, 0, *request, blink::WebURLResponse());

  bool should_replace_current_entry = false;
  WebDataSource* provisional_data_source = frame_->provisionalDataSource();
  WebDataSource* current_data_source = frame_->dataSource();
  WebDataSource* data_source =
      provisional_data_source ? provisional_data_source : current_data_source;

  if (data_source && render_view_->history_list_length_ > 0) {
    should_replace_current_entry = data_source->replacesCurrentHistoryItem();
  }
  Send(new FrameHostMsg_BeginNavigation(
      routing_id_,
      MakeCommonNavigationParams(request, should_replace_current_entry),
      BeginNavigationParams(
          request->httpMethod().latin1(), GetWebURLRequestHeaders(*request),
          GetLoadFlagsForWebURLRequest(*request), request->hasUserGesture()),
      GetRequestBodyForWebURLRequest(*request)));
}
