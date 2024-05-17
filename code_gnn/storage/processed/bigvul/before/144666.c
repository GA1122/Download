void SetAccessibilityModeOnFrame(AccessibilityMode mode,
                                 RenderFrameHost* frame_host) {
  static_cast<RenderFrameHostImpl*>(frame_host)->SetAccessibilityMode(mode);
}
