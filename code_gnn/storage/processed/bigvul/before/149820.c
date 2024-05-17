void LayerTreeHost::RegisterViewportLayers(
    scoped_refptr<Layer> overscroll_elasticity_layer,
    scoped_refptr<Layer> page_scale_layer,
    scoped_refptr<Layer> inner_viewport_scroll_layer,
    scoped_refptr<Layer> outer_viewport_scroll_layer) {
  DCHECK(!inner_viewport_scroll_layer ||
         inner_viewport_scroll_layer != outer_viewport_scroll_layer);
  overscroll_elasticity_layer_ = overscroll_elasticity_layer;
  page_scale_layer_ = page_scale_layer;
  inner_viewport_scroll_layer_ = inner_viewport_scroll_layer;
  outer_viewport_scroll_layer_ = outer_viewport_scroll_layer;
}
