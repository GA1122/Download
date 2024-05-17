InputHandler::ScrollStatus LayerTreeHostImpl::ScrollAnimatedBegin(
    const gfx::Point& viewport_point) {
  InputHandler::ScrollStatus scroll_status;
  scroll_status.main_thread_scrolling_reasons =
      MainThreadScrollingReason::kNotScrollingOnMain;
  ScrollTree& scroll_tree = active_tree_->property_trees()->scroll_tree;
  ScrollNode* scroll_node = scroll_tree.CurrentlyScrollingNode();
  if (scroll_node) {
    gfx::Vector2dF delta;

    if (ScrollAnimationUpdateTarget(scroll_node, delta, base::TimeDelta())) {
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
  if (scroll_status.thread == SCROLL_ON_IMPL_THREAD) {
    ScrollStateData scroll_state_end_data;
    scroll_state_end_data.is_ending = true;
    ScrollState scroll_state_end(scroll_state_end_data);
    ScrollEnd(&scroll_state_end);
  }
  return scroll_status;
}
