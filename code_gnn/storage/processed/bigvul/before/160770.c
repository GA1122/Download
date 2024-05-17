WebView* RenderViewImpl::CreateView(WebLocalFrame* creator,
                                    const WebURLRequest& request,
                                    const WebWindowFeatures& features,
                                    const WebString& frame_name,
                                    WebNavigationPolicy policy,
                                    bool suppress_opener,
                                    WebSandboxFlags sandbox_flags) {
  RenderFrameImpl* creator_frame = RenderFrameImpl::FromWebFrame(creator);
  mojom::CreateNewWindowParamsPtr params = mojom::CreateNewWindowParams::New();
  params->user_gesture =
      WebUserGestureIndicator::IsProcessingUserGesture(creator);
  if (GetContentClient()->renderer()->AllowPopup())
    params->user_gesture = true;
  params->window_container_type = WindowFeaturesToContainerType(features);
  params->session_storage_namespace_id = session_storage_namespace_id_;
  const std::string& frame_name_utf8 = frame_name.Utf8(
      WebString::UTF8ConversionMode::kStrictReplacingErrorsWithFFFD);
  params->frame_name = frame_name_utf8;
  params->opener_suppressed = suppress_opener;
  params->disposition = NavigationPolicyToDisposition(policy);
  if (!request.IsNull()) {
    params->target_url = request.Url();
    params->referrer = GetReferrerFromRequest(creator, request);
  }
  params->features = ConvertWebWindowFeaturesToMojoWindowFeatures(features);

  bool is_background_tab =
      params->disposition == WindowOpenDisposition::NEW_BACKGROUND_TAB;
  bool opened_by_user_gesture = params->user_gesture;

  mojom::CreateNewWindowStatus status;
  mojom::CreateNewWindowReplyPtr reply;
  auto* frame_host = creator_frame->GetFrameHost();
  bool err = !frame_host->CreateNewWindow(std::move(params), &status, &reply);
  if (err || status == mojom::CreateNewWindowStatus::kIgnore)
    return nullptr;

  if (status == mojom::CreateNewWindowStatus::kReuse)
    return webview();

  DCHECK(reply);
  DCHECK_NE(MSG_ROUTING_NONE, reply->route_id);
  DCHECK_NE(MSG_ROUTING_NONE, reply->main_frame_route_id);
  DCHECK_NE(MSG_ROUTING_NONE, reply->main_frame_widget_route_id);

  WebUserGestureIndicator::ConsumeUserGesture(creator);

  bool never_visible = false;

  ResizeParams initial_size = ResizeParams();
  initial_size.screen_info = screen_info_;

  mojom::CreateViewParamsPtr view_params = mojom::CreateViewParams::New();

  view_params->opener_frame_route_id = creator_frame->GetRoutingID();
  DCHECK_EQ(GetRoutingID(), creator_frame->render_view()->GetRoutingID());

  view_params->window_was_created_with_opener = true;
  view_params->renderer_preferences = renderer_preferences_;
  view_params->web_preferences = webkit_preferences_;
  view_params->view_id = reply->route_id;
  view_params->main_frame_routing_id = reply->main_frame_route_id;
  view_params->main_frame_interface_provider =
      std::move(reply->main_frame_interface_provider);
  view_params->main_frame_widget_routing_id = reply->main_frame_widget_route_id;
  view_params->session_storage_namespace_id =
      reply->cloned_session_storage_namespace_id;
  view_params->swapped_out = false;
  view_params->replicated_frame_state.frame_policy.sandbox_flags =
      sandbox_flags;
  view_params->replicated_frame_state.name = frame_name_utf8;
  view_params->devtools_main_frame_token = reply->devtools_main_frame_token;
  view_params->hidden = is_background_tab;
  view_params->never_visible = never_visible;
  view_params->initial_size = initial_size;
  view_params->enable_auto_resize = false;
  view_params->min_size = gfx::Size();
  view_params->max_size = gfx::Size();
  view_params->page_zoom_level = page_zoom_level_;

  RenderWidget::ShowCallback show_callback =
      base::Bind(&RenderFrameImpl::ShowCreatedWindow,
                 base::Unretained(creator_frame), opened_by_user_gesture);

  RenderViewImpl* view = RenderViewImpl::Create(
      compositor_deps_, std::move(view_params), show_callback,
      creator->GetTaskRunner(blink::TaskType::kUnthrottled));

  return view->webview();
}
