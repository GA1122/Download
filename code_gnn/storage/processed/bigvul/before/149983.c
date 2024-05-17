void LayerTreeHostImpl::NotifyTileStateChanged(const Tile* tile) {
  TRACE_EVENT0("cc", "LayerTreeHostImpl::NotifyTileStateChanged");

  if (active_tree_) {
    LayerImpl* layer_impl =
        active_tree_->FindActiveTreeLayerById(tile->layer_id());
    if (layer_impl)
      layer_impl->NotifyTileStateChanged(tile);
  }

  if (pending_tree_) {
    LayerImpl* layer_impl =
        pending_tree_->FindPendingTreeLayerById(tile->layer_id());
    if (layer_impl)
      layer_impl->NotifyTileStateChanged(tile);
  }

  if (active_tree_ && !client_->IsInsideDraw() && tile->required_for_draw()) {
    SetNeedsRedraw();
  }
}
