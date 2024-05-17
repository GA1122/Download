    OnSynchronizeFrameHostVisualProperties(
        const viz::SurfaceId& surface_id,
        const FrameVisualProperties& resize_params) {
  OnSynchronizeVisualProperties(surface_id.local_surface_id(),
                                surface_id.frame_sink_id(), resize_params);
}
