void LayerTreeHost::ApplyPageScaleDeltaFromImplSide(float page_scale_delta) {
  DCHECK(CommitRequested());
  page_scale_factor_ *= page_scale_delta;
}
