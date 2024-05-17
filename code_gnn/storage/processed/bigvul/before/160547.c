void RenderFrameImpl::BindMediaEngagement(
    blink::mojom::MediaEngagementClientAssociatedRequest request) {
  media_engagement_binding_.Bind(std::move(request));
}
