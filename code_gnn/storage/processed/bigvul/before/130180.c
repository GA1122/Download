void RenderFrameHostImpl::OnCreateChildFrame(int new_routing_id,
                                             const std::string& frame_name) {
  RenderFrameHostImpl* new_frame = frame_tree_->AddFrame(
      frame_tree_node_, new_routing_id, frame_name);
  if (delegate_)
    delegate_->RenderFrameCreated(new_frame);
}
