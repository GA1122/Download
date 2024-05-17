void LayerTreeHostImpl::RecreateTileResources() {
  active_tree_->RecreateTileResources();
  if (pending_tree_)
    pending_tree_->RecreateTileResources();
  if (recycle_tree_)
    recycle_tree_->RecreateTileResources();
}
