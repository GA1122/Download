void RenderWidgetHostImpl::RegisterRenderFrameMetadataObserver(
    mojom::RenderFrameMetadataObserverClientRequest
        render_frame_metadata_observer_client_request,
    mojom::RenderFrameMetadataObserverPtr render_frame_metadata_observer) {
  render_frame_metadata_provider_.Bind(
      std::move(render_frame_metadata_observer_client_request),
      std::move(render_frame_metadata_observer));
}
