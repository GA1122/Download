bool LayerTreeHostImpl::PrepareTiles() {
  if (!tile_priorities_dirty_)
    return false;

  client_->WillPrepareTiles();
  bool did_prepare_tiles = tile_manager_.PrepareTiles(global_tile_state_);
  if (did_prepare_tiles)
    tile_priorities_dirty_ = false;
  client_->DidPrepareTiles();
  return did_prepare_tiles;
}
