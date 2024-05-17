bool AreRoutingIDsConsistent(RenderFrameHost* parent_rfh,
                             int32_t child_routing_id) {
  const bool uses_cross_process_frame =
      content::MimeHandlerViewMode::UsesCrossProcessFrame();
  const bool is_child_routing_id_none = (child_routing_id == MSG_ROUTING_NONE);

  bool should_shutdown_process =
      (is_child_routing_id_none == uses_cross_process_frame);

  if (!should_shutdown_process && uses_cross_process_frame) {
    int32_t child_ftn_id = RenderFrameHost::GetFrameTreeNodeIdForRoutingId(
        parent_rfh->GetProcess()->GetID(), child_routing_id);
    auto* child_rfh = content::WebContents::FromRenderFrameHost(parent_rfh)
                          ->UnsafeFindFrameByFrameTreeNodeId(child_ftn_id);
    should_shutdown_process =
        child_rfh && (child_rfh->GetParent() != parent_rfh);
  }
   return !should_shutdown_process;
 }
