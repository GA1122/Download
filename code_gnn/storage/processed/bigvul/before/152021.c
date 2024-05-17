void RenderFrameHostImpl::OnCreateChildFrame(
    int new_routing_id,
    service_manager::mojom::InterfaceProviderRequest
        new_interface_provider_provider_request,
    blink::mojom::DocumentInterfaceBrokerRequest
        document_interface_broker_content_request,
    blink::mojom::DocumentInterfaceBrokerRequest
        document_interface_broker_blink_request,
    blink::WebTreeScopeType scope,
    const std::string& frame_name,
    const std::string& frame_unique_name,
    bool is_created_by_script,
    const base::UnguessableToken& devtools_frame_token,
    const blink::FramePolicy& frame_policy,
    const FrameOwnerProperties& frame_owner_properties,
    const blink::FrameOwnerElementType owner_type) {
  DCHECK(!frame_unique_name.empty());
  DCHECK(new_interface_provider_provider_request.is_pending());
  DCHECK(document_interface_broker_content_request.is_pending());
  DCHECK(document_interface_broker_blink_request.is_pending());
  if (owner_type == blink::FrameOwnerElementType::kNone) {
    bad_message::ReceivedBadMessage(
        GetProcess(), bad_message::RFH_CHILD_FRAME_NEEDS_OWNER_ELEMENT_TYPE);
  }

  if (!is_active() || !IsCurrent() || !render_frame_created_)
    return;

  frame_tree_->AddFrame(
      frame_tree_node_, GetProcess()->GetID(), new_routing_id,
      std::move(new_interface_provider_provider_request),
      std::move(document_interface_broker_content_request),
      std::move(document_interface_broker_blink_request), scope, frame_name,
      frame_unique_name, is_created_by_script, devtools_frame_token,
      frame_policy, frame_owner_properties, was_discarded_, owner_type);
}
