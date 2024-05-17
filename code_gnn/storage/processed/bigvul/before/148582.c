void UpdateAccessibilityModeOnFrame(RenderFrameHost* frame_host) {
  static_cast<RenderFrameHostImpl*>(frame_host)->UpdateAccessibilityMode();
}
