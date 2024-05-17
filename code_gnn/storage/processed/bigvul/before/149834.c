void LayerTreeHost::SetElasticOverscrollFromImplSide(
    gfx::Vector2dF elastic_overscroll) {
  DCHECK(CommitRequested());
  elastic_overscroll_ = elastic_overscroll;
}
