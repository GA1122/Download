void LayerTreeHostImpl::ClearCurrentlyScrollingNode() {
  active_tree_->ClearCurrentlyScrollingNode();
  did_lock_scrolling_layer_ = false;
  scroll_affects_scroll_handler_ = false;
  accumulated_root_overscroll_ = gfx::Vector2dF();
}
