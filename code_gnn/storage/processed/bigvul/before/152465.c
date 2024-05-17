void RenderFrameImpl::OpenURL(std::unique_ptr<blink::WebNavigationInfo> info,
                              bool is_history_navigation_in_new_child) {
  DCHECK(!info->url_request.RequestorOrigin().IsNull());

  WebNavigationPolicy policy = info->navigation_policy;
  FrameHostMsg_OpenURL_Params params;
  params.url = info->url_request.Url();
  params.initiator_origin = info->url_request.RequestorOrigin();
  params.uses_post = IsHttpPost(info->url_request);
  params.resource_request_body =
      GetRequestBodyForWebURLRequest(info->url_request);
  params.extra_headers = GetWebURLRequestHeadersAsString(info->url_request);
  params.referrer =
      RenderViewImpl::GetReferrerFromRequest(frame_, info->url_request);
  params.disposition = RenderViewImpl::NavigationPolicyToDisposition(policy);
  params.triggering_event_info = info->triggering_event_info;
  params.blob_url_token =
      CloneBlobURLToken(info->blob_url_token.get()).PassHandle().release();
  params.should_replace_current_entry =
      info->frame_load_type == WebFrameLoadType::kReplaceCurrentItem &&
      render_view_->history_list_length_;
  params.user_gesture = info->has_transient_user_activation;
  if (GetContentClient()->renderer()->AllowPopup())
    params.user_gesture = true;

  if (is_main_frame_ || policy == blink::kWebNavigationPolicyNewBackgroundTab ||
      policy == blink::kWebNavigationPolicyNewForegroundTab ||
      policy == blink::kWebNavigationPolicyNewWindow ||
      policy == blink::kWebNavigationPolicyNewPopup) {
    WebUserGestureIndicator::ConsumeUserGesture(frame_);
  }

  if (is_history_navigation_in_new_child)
    params.is_history_navigation_in_new_child = true;

  params.href_translate = info->href_translate.Latin1();

  bool current_frame_has_download_sandbox_flag =
      !frame_->IsAllowedToDownloadWithoutUserActivation();
  bool has_download_sandbox_flag =
      info->initiator_frame_has_download_sandbox_flag ||
      current_frame_has_download_sandbox_flag;
  bool from_ad = info->initiator_frame_is_ad || frame_->IsAdSubframe();

  MaybeSetDownloadFramePolicy(
      info->is_opener_navigation, info->url_request,
      frame_->GetSecurityOrigin(), has_download_sandbox_flag,
      info->blocking_downloads_in_sandbox_without_user_activation_enabled,
      from_ad, &params.download_policy);

  Send(new FrameHostMsg_OpenURL(routing_id_, params));
}
