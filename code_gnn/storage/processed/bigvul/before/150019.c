InputHandler::ScrollStatus LayerTreeHostImpl::ScrollBegin(
    ScrollState* scroll_state,
    InputHandler::ScrollInputType type) {
  ScrollStatus scroll_status;
  scroll_status.main_thread_scrolling_reasons =
      MainThreadScrollingReason::kNotScrollingOnMain;
  TRACE_EVENT0("cc", "LayerTreeHostImpl::ScrollBegin");

  ScrollNode* scrolling_node = nullptr;
  bool scroll_on_main_thread = false;

  if (scroll_state->is_in_inertial_phase())
    scrolling_node = CurrentlyScrollingNode();

  if (!scrolling_node) {
    ClearCurrentlyScrollingNode();

    gfx::Point viewport_point(scroll_state->position_x(),
                              scroll_state->position_y());

    gfx::PointF device_viewport_point = gfx::ScalePoint(
        gfx::PointF(viewport_point), active_tree_->device_scale_factor());
    LayerImpl* layer_impl =
        active_tree_->FindLayerThatIsHitByPoint(device_viewport_point);

    if (layer_impl) {
      if (!IsInitialScrollHitTestReliable(layer_impl, device_viewport_point)) {
        scroll_status.thread = SCROLL_UNKNOWN;
        scroll_status.main_thread_scrolling_reasons =
            MainThreadScrollingReason::kFailedHitTest;
        return scroll_status;
      }
    }

    auto* scrolling_layer = FindScrollLayerForDeviceViewportPoint(
        device_viewport_point, type, layer_impl, &scroll_on_main_thread,
        &scroll_status.main_thread_scrolling_reasons);
    ScrollTree& scroll_tree = active_tree_->property_trees()->scroll_tree;
    scrolling_node =
        scrolling_layer ? scroll_tree.Node(scrolling_layer->scroll_tree_index())
                        : nullptr;
  }

  if (scroll_on_main_thread) {
    RecordCompositorSlowScrollMetric(type, MAIN_THREAD);

    scroll_status.thread = SCROLL_ON_MAIN_THREAD;
    return scroll_status;
  } else if (scrolling_node) {
    scroll_affects_scroll_handler_ = active_tree_->have_scroll_event_handlers();
  }

  return ScrollBeginImpl(scroll_state, scrolling_node, type);
}
