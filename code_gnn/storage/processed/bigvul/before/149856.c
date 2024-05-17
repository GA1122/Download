void LayerTreeHost::SetPageScaleFromImplSide(float page_scale) {
  DCHECK(CommitRequested());
  page_scale_factor_ = page_scale;
  SetPropertyTreesNeedRebuild();
}
