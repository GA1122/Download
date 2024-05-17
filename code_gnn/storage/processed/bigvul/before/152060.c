void RenderFrameHostImpl::ProcessBeforeUnloadACK(
    bool proceed,
    bool treat_as_final_ack,
    const base::TimeTicks& renderer_before_unload_start_time,
    const base::TimeTicks& renderer_before_unload_end_time) {
  TRACE_EVENT_ASYNC_END1("navigation", "RenderFrameHostImpl BeforeUnload", this,
                         "FrameTreeNode id",
                         frame_tree_node_->frame_tree_node_id());
  RenderFrameHostImpl* initiator = GetBeforeUnloadInitiator();
  if (!initiator)
    return;

  initiator->ProcessBeforeUnloadACKFromFrame(
      proceed, treat_as_final_ack, this, false  ,
      renderer_before_unload_start_time, renderer_before_unload_end_time);
}
