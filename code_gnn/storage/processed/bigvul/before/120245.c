void LayerTreeHost::PrioritizeTextures(
    const RenderSurfaceLayerList& render_surface_layer_list) {
  if (!contents_texture_manager_)
    return;

  contents_texture_manager_->ClearPriorities();

  size_t memory_for_render_surfaces_metric =
      CalculateMemoryForRenderSurfaces(render_surface_layer_list);

  SetPrioritiesForLayers(render_surface_layer_list);
  SetPrioritiesForSurfaces(memory_for_render_surfaces_metric);

  contents_texture_manager_->PrioritizeTextures();
}
