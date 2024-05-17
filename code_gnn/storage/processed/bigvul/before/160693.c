void RenderFrameImpl::OpenURL(const NavigationPolicyInfo& info,
                              bool send_referrer,
                              bool is_history_navigation_in_new_child) {
  WebNavigationPolicy policy = info.default_policy;
  FrameHostMsg_OpenURL_Params params;
  params.url = info.url_request.Url();
  params.uses_post = IsHttpPost(info.url_request);
  params.resource_request_body =
      GetRequestBodyForWebURLRequest(info.url_request);
  params.extra_headers = GetWebURLRequestHeadersAsString(info.url_request);
  params.referrer = send_referrer ? RenderViewImpl::GetReferrerFromRequest(
                                        frame_, info.url_request)
                                  : content::Referrer();
  params.disposition = RenderViewImpl::NavigationPolicyToDisposition(policy);
  params.triggering_event_info = info.triggering_event_info;
  params.suggested_filename =
      info.url_request.GetSuggestedFilename().has_value()
          ? base::Optional<std::string>(
                info.url_request.GetSuggestedFilename()->Utf8())
          : base::nullopt;

  if (IsBrowserInitiated(pending_navigation_params_.get())) {
    WebDocumentLoader* document_loader = frame_->GetProvisionalDocumentLoader();
    DCHECK(document_loader);
    params.should_replace_current_entry =
        document_loader->ReplacesCurrentHistoryItem();
  } else {
    params.should_replace_current_entry = info.replaces_current_history_item &&
                                          render_view_->history_list_length_;
  }
  params.user_gesture =
      WebUserGestureIndicator::IsProcessingUserGesture(frame_);
  if (GetContentClient()->renderer()->AllowPopup())
    params.user_gesture = true;

  if (ConsumeGestureOnNavigation() ||
      policy == blink::kWebNavigationPolicyNewBackgroundTab ||
      policy == blink::kWebNavigationPolicyNewForegroundTab ||
      policy == blink::kWebNavigationPolicyNewWindow ||
      policy == blink::kWebNavigationPolicyNewPopup) {
    WebUserGestureIndicator::ConsumeUserGesture(frame_);
  }

  if (is_history_navigation_in_new_child)
    params.is_history_navigation_in_new_child = true;

  Send(new FrameHostMsg_OpenURL(routing_id_, params));
}
