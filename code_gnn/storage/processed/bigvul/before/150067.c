void LayerTreeHostImpl::TrackDamageForAllSurfaces(
    const LayerImplList& render_surface_layer_list) {
  size_t render_surface_layer_list_size = render_surface_layer_list.size();
  for (size_t i = 0; i < render_surface_layer_list_size; ++i) {
    size_t surface_index = render_surface_layer_list_size - 1 - i;
    LayerImpl* render_surface_layer = render_surface_layer_list[surface_index];
    RenderSurfaceImpl* render_surface =
        render_surface_layer->GetRenderSurface();
    DCHECK(render_surface);
    render_surface->damage_tracker()->UpdateDamageTrackingState(
        render_surface->layer_list(), render_surface,
        render_surface->SurfacePropertyChangedOnlyFromDescendant(),
        render_surface->content_rect(), render_surface->MaskLayer(),
        render_surface->Filters());
  }
}
