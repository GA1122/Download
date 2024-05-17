void LayerTreeHostImpl::ScrollEnd(ScrollState* scroll_state) {
  DCHECK(scroll_state);
  DCHECK(scroll_state->delta_x() == 0 && scroll_state->delta_y() == 0);

  DistributeScrollDelta(scroll_state);
  browser_controls_offset_manager_->ScrollEnd();
  ClearCurrentlyScrollingNode();
}
