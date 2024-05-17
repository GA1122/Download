void LayerTreeHostImpl::PinchGestureEnd() {
  pinch_gesture_active_ = false;
  if (pinch_gesture_end_should_clear_scrolling_node_) {
    pinch_gesture_end_should_clear_scrolling_node_ = false;
    ClearCurrentlyScrollingNode();
  }
  viewport()->PinchEnd();
  browser_controls_offset_manager_->PinchEnd();
  client_->SetNeedsCommitOnImplThread();
  active_tree_->set_needs_update_draw_properties();
  SetNeedsRedraw();
}
