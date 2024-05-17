void PictureLayer::SetNearestNeighbor(bool nearest_neighbor) {
  if (nearest_neighbor_ == nearest_neighbor)
    return;

  nearest_neighbor_ = nearest_neighbor;
  SetNeedsCommit();
}
