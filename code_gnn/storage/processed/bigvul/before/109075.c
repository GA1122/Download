WebView* RenderViewImpl::createView(
    WebFrame* creator,
    const WebURLRequest& request,
    const WebWindowFeatures& features,
    const WebString& frame_name,
    WebNavigationPolicy policy) {
  if (shared_popup_counter_->data > kMaximumNumberOfUnacknowledgedPopups)
    return NULL;

  ViewHostMsg_CreateWindow_Params params;
  params.opener_id = routing_id_;
  params.user_gesture = creator->isProcessingUserGesture();
  params.window_container_type = WindowFeaturesToContainerType(features);
  params.session_storage_namespace_id = session_storage_namespace_id_;
  params.frame_name = frame_name;
  params.opener_frame_id = creator->identifier();
  params.opener_url = creator->document().url();
  params.opener_security_origin =
      creator->document().securityOrigin().toString().utf8();
  params.opener_suppressed = creator->willSuppressOpenerInNewFrame();
  params.disposition = NavigationPolicyToDisposition(policy);
  if (!request.isNull())
    params.target_url = request.url();

  int32 routing_id = MSG_ROUTING_NONE;
  int32 surface_id = 0;
  int64 cloned_session_storage_namespace_id;

  creator->consumeUserGesture();
  RenderThread::Get()->Send(
      new ViewHostMsg_CreateWindow(params,
                                   &routing_id,
                                   &surface_id,
                                   &cloned_session_storage_namespace_id));
  if (routing_id == MSG_ROUTING_NONE)
    return NULL;

  RenderViewImpl* view = RenderViewImpl::Create(
      0,
      routing_id_,
      renderer_preferences_,
      webkit_preferences_,
      shared_popup_counter_,
      routing_id,
      surface_id,
      cloned_session_storage_namespace_id,
      frame_name,
      true,
      false,
      1,
      screen_info_,
      guest_to_embedder_channel_,
      accessibility_mode_);
  view->opened_by_user_gesture_ = params.user_gesture;

  view->opener_suppressed_ = params.opener_suppressed;

  GURL creator_url(creator->document().securityOrigin().toString().utf8());
  if (!creator_url.is_valid() || !creator_url.IsStandard())
    creator_url = GURL();
  view->creator_url_ = creator_url;

  view->alternate_error_page_url_ = alternate_error_page_url_;

  return view->webview();
}