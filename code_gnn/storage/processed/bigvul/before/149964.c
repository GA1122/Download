bool LayerTreeHostImpl::IsInitialScrollHitTestReliable(
    LayerImpl* layer_impl,
    const gfx::PointF& device_viewport_point) {
  LayerImpl* first_scrolling_layer_or_drawn_scrollbar =
      active_tree_->FindFirstScrollingLayerOrDrawnScrollbarThatIsHitByPoint(
          device_viewport_point);
  if (!first_scrolling_layer_or_drawn_scrollbar)
    return true;

  ScrollNode* closest_scroll_node = nullptr;
  auto& scroll_tree = active_tree_->property_trees()->scroll_tree;
  ScrollNode* scroll_node = scroll_tree.Node(layer_impl->scroll_tree_index());
  for (; scroll_tree.parent(scroll_node);
       scroll_node = scroll_tree.parent(scroll_node)) {
    if (scroll_node->scrollable) {
      closest_scroll_node = scroll_node;
      break;
    }
  }
  if (!closest_scroll_node)
    return false;

  if (first_scrolling_layer_or_drawn_scrollbar->scrollable()) {
    return closest_scroll_node->id ==
           first_scrolling_layer_or_drawn_scrollbar->scroll_tree_index();
  }

  DCHECK(first_scrolling_layer_or_drawn_scrollbar->IsDrawnScrollbar());
  return false;
}
