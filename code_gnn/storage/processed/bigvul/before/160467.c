void RenderFrameHostImpl::OnOpenURL(const FrameHostMsg_OpenURL_Params& params) {
  GURL validated_url(params.url);
  GetProcess()->FilterURL(false, &validated_url);
  if (!ChildProcessSecurityPolicyImpl::GetInstance()->CanReadRequestBody(
          GetSiteInstance(), params.resource_request_body)) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RFH_ILLEGAL_UPLOAD_PARAMS);
    return;
  }

  if (params.is_history_navigation_in_new_child) {
    if (frame_tree_node_->navigator()->NavigateNewChildFrame(this,
                                                             validated_url))
      return;
  }

  TRACE_EVENT1("navigation", "RenderFrameHostImpl::OpenURL", "url",
               validated_url.possibly_invalid_spec());

  frame_tree_node_->navigator()->RequestOpenURL(
      this, validated_url, params.uses_post, params.resource_request_body,
      params.extra_headers, params.referrer, params.disposition,
      params.should_replace_current_entry, params.user_gesture,
      params.triggering_event_info, params.suggested_filename);
}
