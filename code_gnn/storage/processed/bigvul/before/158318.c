void RenderWidgetHostImpl::OnLocalSurfaceIdChanged(
    const cc::RenderFrameMetadata& metadata) {
  DidUpdateVisualProperties(metadata);
}
