void CreateFrameResourceCoordinator(
    RenderFrameHostImpl* render_frame_host,
    resource_coordinator::mojom::FrameCoordinationUnitRequest request) {
  render_frame_host->GetFrameResourceCoordinator()->AddBinding(
      std::move(request));
}
