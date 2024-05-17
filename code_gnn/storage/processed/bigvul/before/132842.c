bool PictureLayerImpl::HasValidTilePriorities() const {
  return IsOnActiveOrPendingTree() && IsDrawnRenderSurfaceLayerListMember();
}
