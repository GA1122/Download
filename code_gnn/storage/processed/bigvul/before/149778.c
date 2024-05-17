void LayerTreeHost::BuildPropertyTreesForTesting() {
  PropertyTreeBuilder::PreCalculateMetaInformation(root_layer());
  gfx::Transform identity_transform;
  PropertyTreeBuilder::BuildPropertyTrees(
      root_layer(), page_scale_layer(), inner_viewport_scroll_layer(),
      outer_viewport_scroll_layer(), overscroll_elasticity_layer(),
      elastic_overscroll(), page_scale_factor(), device_scale_factor(),
      gfx::Rect(device_viewport_size()), identity_transform, property_trees());
}
