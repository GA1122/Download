void RenderFrameHostImpl::OnDidStartProvisionalLoadForFrame(
    int parent_routing_id,
    bool is_main_frame,
    const GURL& url) {
  frame_tree_node_->navigator()->DidStartProvisionalLoad(
      this, parent_routing_id, is_main_frame, url);
}
