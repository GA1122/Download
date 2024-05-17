bool LayerTreeHostImpl::IsCurrentlyScrollingLayerAt(
    const gfx::Point& viewport_point,
    InputHandler::ScrollInputType type) const {
  auto* scrolling_node = CurrentlyScrollingNode();
  if (!scrolling_node)
    return false;

  gfx::PointF device_viewport_point = gfx::ScalePoint(
      gfx::PointF(viewport_point), active_tree_->device_scale_factor());

  LayerImpl* layer_impl =
      active_tree_->FindLayerThatIsHitByPoint(device_viewport_point);

  bool scroll_on_main_thread = false;
  uint32_t main_thread_scrolling_reasons;
  LayerImpl* test_layer_impl = FindScrollLayerForDeviceViewportPoint(
      device_viewport_point, type, layer_impl, &scroll_on_main_thread,
      &main_thread_scrolling_reasons);

  if (scroll_on_main_thread)
    return false;

  int test_scroll_tree_index = test_layer_impl->scroll_tree_index();
  if (scrolling_node->id == test_scroll_tree_index)
    return true;

  if (scrolling_node->scrolls_inner_viewport ||
      scrolling_node->scrolls_outer_viewport) {
    return test_layer_impl == viewport()->MainScrollLayer();
  }

  return false;
}
