void RenderViewHostImpl::UpdateFrameTree(
    int process_id,
    int route_id,
    const std::string& frame_tree) {
  DCHECK(is_swapped_out_);

  frame_tree_ = frame_tree;
  Send(new ViewMsg_UpdateFrameTree(GetRoutingID(),
                                   process_id,
                                   route_id,
                                   frame_tree_));
}
