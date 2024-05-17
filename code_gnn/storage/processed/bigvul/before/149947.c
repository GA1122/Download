LayerImpl* LayerTreeHostImpl::FindScrollLayerForDeviceViewportPoint(
    const gfx::PointF& device_viewport_point,
    InputHandler::ScrollInputType type,
    LayerImpl* layer_impl,
    bool* scroll_on_main_thread,
    uint32_t* main_thread_scrolling_reasons) const {
  DCHECK(scroll_on_main_thread);
  DCHECK(main_thread_scrolling_reasons);
  *main_thread_scrolling_reasons =
      MainThreadScrollingReason::kNotScrollingOnMain;

  ScrollTree& scroll_tree = active_tree_->property_trees()->scroll_tree;
  ScrollNode* impl_scroll_node = nullptr;
  if (layer_impl) {
    ScrollNode* scroll_node = scroll_tree.Node(layer_impl->scroll_tree_index());
    for (; scroll_tree.parent(scroll_node);
         scroll_node = scroll_tree.parent(scroll_node)) {
      ScrollStatus status =
          TryScroll(device_viewport_point, type, scroll_tree, scroll_node);
      if (IsMainThreadScrolling(status, scroll_node)) {
        *scroll_on_main_thread = true;
        *main_thread_scrolling_reasons = status.main_thread_scrolling_reasons;
        return active_tree_->LayerById(scroll_node->owning_layer_id);
      }

      if (status.thread == InputHandler::SCROLL_ON_IMPL_THREAD &&
          !impl_scroll_node) {
        impl_scroll_node = scroll_node;
      }
    }
  }

  bool scrolls_inner_viewport =
      impl_scroll_node && impl_scroll_node->scrolls_inner_viewport;
  bool scrolls_outer_viewport =
      impl_scroll_node && impl_scroll_node->scrolls_outer_viewport;
  if (!impl_scroll_node || scrolls_inner_viewport || scrolls_outer_viewport)
    impl_scroll_node = OuterViewportScrollNode();

  if (impl_scroll_node) {
    ScrollStatus status =
        TryScroll(device_viewport_point, type, scroll_tree, impl_scroll_node);
    if (IsMainThreadScrolling(status, impl_scroll_node)) {
      *scroll_on_main_thread = true;
      *main_thread_scrolling_reasons = status.main_thread_scrolling_reasons;
    }
  }

  if (!impl_scroll_node)
    return nullptr;
  return active_tree_->LayerById(impl_scroll_node->owning_layer_id);
}
