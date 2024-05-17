void ResetAccessibility(RenderFrameHost* rfh) {
  static_cast<RenderFrameHostImpl*>(rfh)->AccessibilityReset();
}
