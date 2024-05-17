void RenderFrameHostImpl::SetAccessibilityCallbackForTesting(
    const base::Callback<void(RenderFrameHostImpl*, ax::mojom::Event, int)>&
        callback) {
  accessibility_testing_callback_ = callback;
}
