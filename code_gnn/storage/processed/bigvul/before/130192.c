void RenderFrameHostImpl::OnOpenURL(
    const FrameHostMsg_OpenURL_Params& params) {
  GURL validated_url(params.url);
  GetProcess()->FilterURL(false, &validated_url);

  frame_tree_node_->navigator()->RequestOpenURL(
      this, validated_url, params.referrer, params.disposition,
      params.should_replace_current_entry, params.user_gesture);
}
