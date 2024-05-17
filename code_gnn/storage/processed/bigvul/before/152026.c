void RenderFrameHostImpl::OnDidChangeFrameOwnerProperties(
    int32_t frame_routing_id,
    const FrameOwnerProperties& properties) {
  FrameTreeNode* child = FindAndVerifyChild(
      frame_routing_id, bad_message::RFH_OWNER_PROPERTY);
  if (!child)
    return;

  bool has_display_none_property_changed =
      properties.is_display_none !=
      child->frame_owner_properties().is_display_none;

  child->set_frame_owner_properties(properties);

  child->render_manager()->OnDidUpdateFrameOwnerProperties(properties);
  if (has_display_none_property_changed) {
    delegate_->DidChangeDisplayState(
        child->current_frame_host(),
        properties.is_display_none  );
  }
}
