void RenderFrameImpl::OnSnapshotAccessibilityTree(int callback_id) {
  ui::AXTreeUpdate<AXContentNodeData> response;
  RendererAccessibility::SnapshotAccessibilityTree(this, &response);
  Send(new AccessibilityHostMsg_SnapshotResponse(
      routing_id_, callback_id, response));
}
