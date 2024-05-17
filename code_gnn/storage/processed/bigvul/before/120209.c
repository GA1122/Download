size_t LayerTreeHost::CalculateMemoryForRenderSurfaces(
    const RenderSurfaceLayerList& update_list) {
  size_t readback_bytes = 0;
  size_t max_background_texture_bytes = 0;
  size_t contents_texture_bytes = 0;

  for (size_t i = 1; i < update_list.size(); ++i) {
    Layer* render_surface_layer = update_list.at(i);
    RenderSurface* render_surface = render_surface_layer->render_surface();

    size_t bytes =
        Resource::MemorySizeBytes(render_surface->content_rect().size(),
                                  RGBA_8888);
    contents_texture_bytes += bytes;

    if (render_surface_layer->background_filters().IsEmpty())
      continue;

    if (bytes > max_background_texture_bytes)
      max_background_texture_bytes = bytes;
    if (!readback_bytes) {
      readback_bytes = Resource::MemorySizeBytes(device_viewport_size_,
                                                 RGBA_8888);
    }
  }
  return readback_bytes + max_background_texture_bytes + contents_texture_bytes;
}
