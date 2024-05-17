static gfx::Vector2dF ScrollNodeWithLocalDelta(
    ScrollNode* scroll_node,
    const gfx::Vector2dF& local_delta,
    float page_scale_factor,
    LayerTreeImpl* layer_tree_impl) {
  ScrollTree& scroll_tree = layer_tree_impl->property_trees()->scroll_tree;
  gfx::ScrollOffset previous_offset =
      scroll_tree.current_scroll_offset(scroll_node->owning_layer_id);
  gfx::Vector2dF delta = local_delta;
  delta.Scale(1.f / page_scale_factor);
  scroll_tree.ScrollBy(scroll_node, delta, layer_tree_impl);
  gfx::ScrollOffset scrolled =
      scroll_tree.current_scroll_offset(scroll_node->owning_layer_id) -
      previous_offset;
  gfx::Vector2dF consumed_scroll(scrolled.x(), scrolled.y());
  consumed_scroll.Scale(page_scale_factor);

  return consumed_scroll;
}
