void LayerTreeHostImpl::PinchGestureBegin() {
  pinch_gesture_active_ = true;
  client_->RenewTreePriority();
  pinch_gesture_end_should_clear_scrolling_node_ = !CurrentlyScrollingNode();

  active_tree_->SetCurrentlyScrollingNode(OuterViewportScrollNode());
  browser_controls_offset_manager_->PinchBegin();
}
