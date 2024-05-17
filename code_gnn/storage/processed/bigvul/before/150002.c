void LayerTreeHostImpl::ReleaseTileResources() {
  active_tree_->ReleaseTileResources();
  if (pending_tree_)
    pending_tree_->ReleaseTileResources();
  if (recycle_tree_)
    recycle_tree_->ReleaseTileResources();
}
