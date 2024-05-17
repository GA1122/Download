gfx::Vector2dF LayerTreeHostImpl::ComputeScrollDelta(
    ScrollNode* scroll_node,
    const gfx::Vector2dF& delta) {
  ScrollTree& scroll_tree = active_tree()->property_trees()->scroll_tree;
  float scale_factor = active_tree()->current_page_scale_factor();

  gfx::Vector2dF adjusted_scroll(delta);
  adjusted_scroll.Scale(1.f / scale_factor);
  if (!scroll_node->user_scrollable_horizontal)
    adjusted_scroll.set_x(0);
  if (!scroll_node->user_scrollable_vertical)
    adjusted_scroll.set_y(0);

  gfx::ScrollOffset old_offset =
      scroll_tree.current_scroll_offset(scroll_node->owning_layer_id);
  gfx::ScrollOffset new_offset = scroll_tree.ClampScrollOffsetToLimits(
      old_offset + gfx::ScrollOffset(adjusted_scroll), scroll_node);

  gfx::ScrollOffset scrolled = new_offset - old_offset;
  return gfx::Vector2dF(scrolled.x(), scrolled.y());
}
