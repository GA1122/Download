PictureLayerTiling* PictureLayerImpl::GetRecycledTwinTiling(
    const PictureLayerTiling* tiling) {
  PictureLayerImpl* recycled_twin = GetRecycledTwinLayer();
  if (!recycled_twin || !recycled_twin->tilings_)
    return nullptr;
  return recycled_twin->tilings_->FindTilingWithScale(tiling->contents_scale());
}
