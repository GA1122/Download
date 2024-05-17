FrameTreeNode* FindOpener(const WebContents::CreateParams& params) {
  FrameTreeNode* opener_node = nullptr;
  if (params.opener_render_frame_id != MSG_ROUTING_NONE) {
    RenderFrameHostImpl* opener_rfh = RenderFrameHostImpl::FromID(
        params.opener_render_process_id, params.opener_render_frame_id);
    if (opener_rfh)
      opener_node = opener_rfh->frame_tree_node();
  }
  return opener_node;
}
