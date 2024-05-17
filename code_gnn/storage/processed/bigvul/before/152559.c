void RenderFrameImpl::VisibilityChanged(
    blink::mojom::FrameVisibility visibility) {
  GetFrameHost()->VisibilityChanged(visibility);
}
