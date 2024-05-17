void LayerTreeHostImpl::ApplyScroll(ScrollNode* scroll_node,
                                    ScrollState* scroll_state) {
  DCHECK(scroll_node && scroll_state);
  gfx::Point viewport_point(scroll_state->position_x(),
                            scroll_state->position_y());
  const gfx::Vector2dF delta(scroll_state->delta_x(), scroll_state->delta_y());
  gfx::Vector2dF applied_delta;
  gfx::Vector2dF delta_applied_to_content;
  const float kEpsilon = 0.1f;

  bool scrolls_main_viewport_scroll_layer =
      viewport()->MainScrollLayer() &&
      viewport()->MainScrollLayer()->scroll_tree_index() == scroll_node->id;

  bool scrolls_inner_viewport = scroll_node->scrolls_inner_viewport;

  if (scrolls_main_viewport_scroll_layer || scrolls_inner_viewport) {
    Viewport::ScrollResult result = viewport()->ScrollBy(
        delta, viewport_point, scroll_state->is_direct_manipulation(),
        !wheel_scrolling_, scrolls_main_viewport_scroll_layer);

    applied_delta = result.consumed_delta;
    delta_applied_to_content = result.content_scrolled_delta;
  } else {
    applied_delta = ScrollSingleNode(
        scroll_node, delta, viewport_point,
        scroll_state->is_direct_manipulation(),
        &scroll_state->layer_tree_impl()->property_trees()->scroll_tree);
  }

  bool scrolled = std::abs(applied_delta.x()) > kEpsilon;
  scrolled = scrolled || std::abs(applied_delta.y()) > kEpsilon;
  if (!scrolled) {
    if (scrolls_main_viewport_scroll_layer)
      scroll_state->ConsumeDelta(applied_delta.x(), applied_delta.y());
    return;
  }

  if (!scrolls_main_viewport_scroll_layer && !scrolls_inner_viewport) {
    float angle_threshold = 45;
    if (MathUtil::SmallestAngleBetweenVectors(applied_delta, delta) <
        angle_threshold) {
      applied_delta = delta;
    } else {
      applied_delta = MathUtil::ProjectVector(delta, applied_delta);
    }
    delta_applied_to_content = applied_delta;
  }

  scroll_state->set_caused_scroll(
      std::abs(delta_applied_to_content.x()) > kEpsilon,
      std::abs(delta_applied_to_content.y()) > kEpsilon);
  scroll_state->ConsumeDelta(applied_delta.x(), applied_delta.y());

  scroll_state->set_current_native_scrolling_node(scroll_node);
}
