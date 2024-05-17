void PictureLayerImpl::ReleaseResources() {
  if (tilings_)
    tilings_ = CreatePictureLayerTilingSet();
  ResetRasterScale();

  layer_tree_impl()->set_needs_update_draw_properties();
}
