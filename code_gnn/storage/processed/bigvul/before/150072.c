void LayerTreeHostImpl::UpdateRootLayerStateForSynchronousInputHandler() {
  if (!input_handler_client_)
    return;
  input_handler_client_->UpdateRootLayerStateForSynchronousInputHandler(
      active_tree_->TotalScrollOffset(), active_tree_->TotalMaxScrollOffset(),
      active_tree_->ScrollableSize(), active_tree_->current_page_scale_factor(),
      active_tree_->min_page_scale_factor(),
      active_tree_->max_page_scale_factor());
}
