void RenderFrameHostImpl::OnOpenURL(const FrameHostMsg_OpenURL_Params& params) {
  GURL validated_url(params.url);
  GetProcess()->FilterURL(false, &validated_url);

  if (params.is_history_navigation_in_new_child) {
    if (frame_tree_node_->navigator()->NavigateNewChildFrame(this,
                                                             validated_url))
      return;
  }

  TRACE_EVENT1("navigation", "RenderFrameHostImpl::OpenURL", "url",
               validated_url.possibly_invalid_spec());

  bool kForceNewProcessForNewContents = true;

  frame_tree_node_->navigator()->RequestOpenURL(
      this, validated_url, params.uses_post, params.resource_request_body,
      params.extra_headers, params.referrer, params.disposition,
      kForceNewProcessForNewContents, params.should_replace_current_entry,
      params.user_gesture);
}
