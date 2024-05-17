InputHandler::ScrollStatus LayerTreeHostImpl::TryScroll(
    const gfx::PointF& screen_space_point,
    InputHandler::ScrollInputType type,
    const ScrollTree& scroll_tree,
    ScrollNode* scroll_node) const {
  InputHandler::ScrollStatus scroll_status;
  scroll_status.main_thread_scrolling_reasons =
      MainThreadScrollingReason::kNotScrollingOnMain;
  if (!!scroll_node->main_thread_scrolling_reasons) {
    TRACE_EVENT0("cc", "LayerImpl::TryScroll: Failed ShouldScrollOnMainThread");
    scroll_status.thread = InputHandler::SCROLL_ON_MAIN_THREAD;
    scroll_status.main_thread_scrolling_reasons =
        scroll_node->main_thread_scrolling_reasons;
    return scroll_status;
  }

  gfx::Transform screen_space_transform =
      scroll_tree.ScreenSpaceTransform(scroll_node->id);
  if (!screen_space_transform.IsInvertible()) {
    TRACE_EVENT0("cc", "LayerImpl::TryScroll: Ignored NonInvertibleTransform");
    scroll_status.thread = InputHandler::SCROLL_IGNORED;
    scroll_status.main_thread_scrolling_reasons =
        MainThreadScrollingReason::kNonInvertibleTransform;
    return scroll_status;
  }

  if (!scroll_node->non_fast_scrollable_region.IsEmpty()) {
    bool clipped = false;
    gfx::Transform inverse_screen_space_transform(
        gfx::Transform::kSkipInitialization);
    if (!screen_space_transform.GetInverse(&inverse_screen_space_transform)) {
    }

    gfx::PointF hit_test_point_in_layer_space = MathUtil::ProjectPoint(
        inverse_screen_space_transform, screen_space_point, &clipped);
    if (!clipped && scroll_node->non_fast_scrollable_region.Contains(
                        gfx::ToRoundedPoint(hit_test_point_in_layer_space))) {
      TRACE_EVENT0("cc",
                   "LayerImpl::tryScroll: Failed NonFastScrollableRegion");
      scroll_status.thread = InputHandler::SCROLL_ON_MAIN_THREAD;
      scroll_status.main_thread_scrolling_reasons =
          MainThreadScrollingReason::kNonFastScrollableRegion;
      return scroll_status;
    }
  }

  if (!scroll_node->scrollable) {
    TRACE_EVENT0("cc", "LayerImpl::tryScroll: Ignored not scrollable");
    scroll_status.thread = InputHandler::SCROLL_IGNORED;
    scroll_status.main_thread_scrolling_reasons =
        MainThreadScrollingReason::kNotScrollable;
    return scroll_status;
  }

  gfx::ScrollOffset max_scroll_offset =
      scroll_tree.MaxScrollOffset(scroll_node->id);
  if (max_scroll_offset.x() <= 0 && max_scroll_offset.y() <= 0) {
    TRACE_EVENT0("cc",
                 "LayerImpl::tryScroll: Ignored. Technically scrollable,"
                 " but has no affordance in either direction.");
    scroll_status.thread = InputHandler::SCROLL_IGNORED;
    scroll_status.main_thread_scrolling_reasons =
        MainThreadScrollingReason::kNotScrollable;
    return scroll_status;
  }

  scroll_status.thread = InputHandler::SCROLL_ON_IMPL_THREAD;
  return scroll_status;
}
