void LayerTreeHostImpl::DidModifyTilePriorities() {
  tile_priorities_dirty_ = true;
  client_->SetNeedsPrepareTilesOnImplThread();
}
