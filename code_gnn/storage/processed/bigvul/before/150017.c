bool LayerTreeHostImpl::ScrollAnimationCreate(ScrollNode* scroll_node,
                                              const gfx::Vector2dF& delta,
                                              base::TimeDelta delayed_by) {
  ScrollTree& scroll_tree = active_tree_->property_trees()->scroll_tree;

  const float kEpsilon = 0.1f;
  bool scroll_animated =
      (std::abs(delta.x()) > kEpsilon || std::abs(delta.y()) > kEpsilon);
  if (!scroll_animated) {
    scroll_tree.ScrollBy(scroll_node, delta, active_tree());
    return false;
  }

  scroll_tree.set_currently_scrolling_node(scroll_node->id);

  gfx::ScrollOffset current_offset =
      scroll_tree.current_scroll_offset(scroll_node->owning_layer_id);
  gfx::ScrollOffset target_offset = scroll_tree.ClampScrollOffsetToLimits(
      current_offset + gfx::ScrollOffset(delta), scroll_node);
  DCHECK_EQ(
      ElementId(
          active_tree()->LayerById(scroll_node->owning_layer_id)->element_id()),
      scroll_node->element_id);

  mutator_host_->ImplOnlyScrollAnimationCreate(
      scroll_node->element_id, target_offset, current_offset, delayed_by);

  SetNeedsOneBeginImplFrame();

  return true;
}
