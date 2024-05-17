InputHandler::ScrollStatus LayerTreeHostImpl::ScrollAnimated(
    const gfx::Point& viewport_point,
    const gfx::Vector2dF& scroll_delta,
    base::TimeDelta delayed_by) {
  InputHandler::ScrollStatus scroll_status;
  scroll_status.main_thread_scrolling_reasons =
      MainThreadScrollingReason::kNotScrollingOnMain;
  ScrollTree& scroll_tree = active_tree_->property_trees()->scroll_tree;
  ScrollNode* scroll_node = scroll_tree.CurrentlyScrollingNode();
  if (scroll_node) {
    gfx::Vector2dF delta = scroll_delta;
    if (!scroll_node->user_scrollable_horizontal)
      delta.set_x(0);
    if (!scroll_node->user_scrollable_vertical)
      delta.set_y(0);

    if (ScrollAnimationUpdateTarget(scroll_node, delta, delayed_by)) {
      scroll_status.thread = SCROLL_ON_IMPL_THREAD;
    } else {
      scroll_status.thread = SCROLL_IGNORED;
      scroll_status.main_thread_scrolling_reasons =
          MainThreadScrollingReason::kNotScrollable;
    }
    return scroll_status;
  }

  ScrollStateData scroll_state_data;
  scroll_state_data.position_x = viewport_point.x();
  scroll_state_data.position_y = viewport_point.y();
  ScrollState scroll_state(scroll_state_data);

  scroll_status = ScrollBegin(&scroll_state, WHEEL);
  scroll_node = scroll_tree.CurrentlyScrollingNode();
  if (scroll_status.thread == SCROLL_ON_IMPL_THREAD && scroll_node) {
    gfx::Vector2dF pending_delta = scroll_delta;
    for (; scroll_tree.parent(scroll_node);
         scroll_node = scroll_tree.parent(scroll_node)) {
      if (!scroll_node->scrollable)
        continue;

      bool scrolls_main_viewport_scroll_layer =
          viewport()->MainScrollLayer() &&
          viewport()->MainScrollLayer()->scroll_tree_index() == scroll_node->id;
      if (scrolls_main_viewport_scroll_layer) {
        gfx::Vector2dF scrolled =
            viewport()->ScrollAnimated(pending_delta, delayed_by);
        if (scrolled == pending_delta)
          return scroll_status;
        break;
      }

      gfx::Vector2dF scroll_delta =
          ComputeScrollDelta(scroll_node, pending_delta);
      if (ScrollAnimationCreate(scroll_node, scroll_delta, delayed_by))
        return scroll_status;

      pending_delta -= scroll_delta;
    }
  }
  scroll_state.set_is_ending(true);
  ScrollEnd(&scroll_state);
  return scroll_status;
}
