InputHandlerScrollResult LayerTreeHostImpl::ScrollBy(
    ScrollState* scroll_state) {
  DCHECK(scroll_state);

  TRACE_EVENT0("cc", "LayerTreeHostImpl::ScrollBy");
  if (!CurrentlyScrollingNode())
    return InputHandlerScrollResult();

  float initial_top_controls_offset =
      browser_controls_offset_manager_->ControlsTopOffset();

  scroll_state->set_delta_consumed_for_scroll_sequence(
      did_lock_scrolling_layer_);
  scroll_state->set_is_direct_manipulation(!wheel_scrolling_);
  scroll_state->set_current_native_scrolling_node(
      active_tree()->property_trees()->scroll_tree.CurrentlyScrollingNode());

  DistributeScrollDelta(scroll_state);

  active_tree_->SetCurrentlyScrollingNode(
      scroll_state->current_native_scrolling_node());
  did_lock_scrolling_layer_ =
      scroll_state->delta_consumed_for_scroll_sequence();

  bool did_scroll_x = scroll_state->caused_scroll_x();
  bool did_scroll_y = scroll_state->caused_scroll_y();
  bool did_scroll_content = did_scroll_x || did_scroll_y;
  if (did_scroll_content) {
    if (scroll_affects_scroll_handler())
      NotifySwapPromiseMonitorsOfForwardingToMainThread();
    client_->SetNeedsCommitOnImplThread();
    SetNeedsRedraw();
    client_->RenewTreePriority();
  }

  if (did_scroll_x)
    accumulated_root_overscroll_.set_x(0);
  if (did_scroll_y)
    accumulated_root_overscroll_.set_y(0);
  gfx::Vector2dF unused_root_delta(scroll_state->delta_x(),
                                   scroll_state->delta_y());

  if (InnerViewportScrollLayer()) {
    if (!InnerViewportScrollLayer()->user_scrollable_horizontal())
      unused_root_delta.set_x(0);
    if (!InnerViewportScrollLayer()->user_scrollable_vertical())
      unused_root_delta.set_y(0);
  }

  accumulated_root_overscroll_ += unused_root_delta;

  bool did_scroll_top_controls =
      initial_top_controls_offset !=
      browser_controls_offset_manager_->ControlsTopOffset();

  InputHandlerScrollResult scroll_result;
  scroll_result.did_scroll = did_scroll_content || did_scroll_top_controls;
  scroll_result.did_overscroll_root = !unused_root_delta.IsZero();
  scroll_result.accumulated_root_overscroll = accumulated_root_overscroll_;
  scroll_result.unused_scroll_delta = unused_root_delta;

  if (scroll_result.did_scroll) {
    UpdateRootLayerStateForSynchronousInputHandler();
  }

  Mutate(CurrentBeginFrameArgs().frame_time);

  return scroll_result;
}
