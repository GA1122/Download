void PictureLayerImpl::DidBecomeActive() {
  LayerImpl::DidBecomeActive();
  layer_tree_impl()->DidModifyTilePriorities();
}
