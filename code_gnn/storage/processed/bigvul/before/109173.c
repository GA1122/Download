bool RenderViewImpl::willCheckAndDispatchMessageEvent(
    WebKit::WebFrame* sourceFrame,
    WebKit::WebFrame* targetFrame,
    WebKit::WebSecurityOrigin target_origin,
    WebKit::WebDOMMessageEvent event) {
  if (!is_swapped_out_)
    return false;

  ViewMsg_PostMessage_Params params;
  params.data = event.data().toString();
  params.source_origin = event.origin();
  if (!target_origin.isNull())
    params.target_origin = target_origin.toString();

  params.source_routing_id = MSG_ROUTING_NONE;
  RenderViewImpl* source_view = FromWebView(sourceFrame->view());
  if (source_view)
    params.source_routing_id = source_view->routing_id();
  params.source_frame_id = sourceFrame->identifier();

  params.target_process_id = target_process_id_;
  params.target_routing_id = target_routing_id_;

  std::map<int,int>::iterator it = active_frame_id_map_.find(
      targetFrame->identifier());
  if (it != active_frame_id_map_.end()) {
    params.target_frame_id = it->second;
  } else {
    params.target_frame_id = 0;
  }

  Send(new ViewHostMsg_RouteMessageEvent(routing_id_, params));
  return true;
}
