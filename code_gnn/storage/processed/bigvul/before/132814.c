bool PictureLayerImpl::AllTilesRequiredForActivationAreReadyToDraw() const {
  if (!layer_tree_impl()->IsPendingTree())
    return true;

  return AllTilesRequiredAreReadyToDraw(
      &PictureLayerTiling::IsTileRequiredForActivationIfVisible);
}
