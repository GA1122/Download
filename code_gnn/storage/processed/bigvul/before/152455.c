void RenderFrameImpl::OnSnapshotAccessibilityTree(int callback_id,
                                                  ui::AXMode ax_mode) {
  AXContentTreeUpdate response;
  RenderAccessibilityImpl::SnapshotAccessibilityTree(this, &response, ax_mode);
  Send(new AccessibilityHostMsg_SnapshotResponse(
      routing_id_, callback_id, response));
}
