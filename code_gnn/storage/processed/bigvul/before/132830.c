void PictureLayerImpl::GetAllTilesForTracing(
    std::set<const Tile*>* tiles) const {
  if (!tilings_)
    return;
  tilings_->GetAllTilesForTracing(tiles);
}
