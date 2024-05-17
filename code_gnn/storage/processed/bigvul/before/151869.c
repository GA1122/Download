FrameTreeNode* RenderFrameHostImpl::AddChild(
    std::unique_ptr<FrameTreeNode> child,
    int process_id,
    int frame_routing_id) {
  CHECK_EQ(process_id, GetProcess()->GetID());

  child->render_manager()->Init(GetSiteInstance(),
                                render_view_host()->GetRoutingID(),
                                frame_routing_id, MSG_ROUTING_NONE, false);

  frame_tree_node_->render_manager()->CreateProxiesForChildFrame(child.get());

  child->current_frame_host()->SetOriginOfNewFrame(GetLastCommittedOrigin());

  children_.push_back(std::move(child));
  return children_.back().get();
}
