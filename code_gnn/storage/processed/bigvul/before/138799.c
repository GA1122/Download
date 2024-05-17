void RenderFrameHostImpl::SetAccessibilityCallbackForTesting(
    const base::Callback<void(RenderFrameHostImpl*, ui::AXEvent, int)>&
        callback) {
  accessibility_testing_callback_ = callback;
}
