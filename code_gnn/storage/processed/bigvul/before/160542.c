void RenderFrameImpl::BindEngagement(
    blink::mojom::EngagementClientAssociatedRequest request) {
  engagement_binding_.Bind(std::move(request));
}
