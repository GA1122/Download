void RenderFrameHostImpl::OnRenderFallbackContentInParentProcess() {
  bool is_object_type =
      frame_tree_node()->current_replication_state().frame_owner_element_type ==
      blink::FrameOwnerElementType::kObject;
  if (!is_object_type) {
    bad_message::ReceivedBadMessage(
        GetProcess(), bad_message::RFH_CANNOT_RENDER_FALLBACK_CONTENT);
    return;
  }

  auto* rfh = frame_tree_node()->current_frame_host();
  if (rfh->GetSiteInstance() == rfh->GetParent()->GetSiteInstance()) {
    rfh->Send(new FrameMsg_RenderFallbackContent(rfh->GetRoutingID()));
  } else if (auto* proxy =
                 frame_tree_node()->render_manager()->GetProxyToParent()) {
    proxy->Send(new FrameMsg_RenderFallbackContent(proxy->GetRoutingID()));
  }
}
