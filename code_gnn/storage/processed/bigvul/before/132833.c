TilePriority::PriorityBin PictureLayerImpl::GetMaxTilePriorityBin() const {
  if (!HasValidTilePriorities())
    return TilePriority::EVENTUALLY;
  return TilePriority::NOW;
}
