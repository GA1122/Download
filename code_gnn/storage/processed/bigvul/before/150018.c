bool LayerTreeHostImpl::ScrollAnimationUpdateTarget(
    ScrollNode* scroll_node,
    const gfx::Vector2dF& scroll_delta,
    base::TimeDelta delayed_by) {
  DCHECK_EQ(
      ElementId(
          active_tree()->LayerById(scroll_node->owning_layer_id)->element_id()),
      scroll_node->element_id);

  return mutator_host_->ImplOnlyScrollAnimationUpdateTarget(
      scroll_node->element_id, scroll_delta,
      active_tree_->property_trees()->scroll_tree.MaxScrollOffset(
          scroll_node->id),
      CurrentBeginFrameArgs().frame_time, delayed_by);
}
