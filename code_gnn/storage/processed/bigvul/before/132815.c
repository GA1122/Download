bool PictureLayerImpl::AllTilesRequiredForDrawAreReadyToDraw() const {
  if (!layer_tree_impl()->IsActiveTree())
    return true;

  return AllTilesRequiredAreReadyToDraw(
      &PictureLayerTiling::IsTileRequiredForDrawIfVisible);
}
