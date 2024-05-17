void LayerTreeHost::ApplyPageScaleDeltaFromImplSide(
    float page_scale_delta) {
  DCHECK(CommitRequested());
  if (page_scale_delta == 1.f)
    return;
  float page_scale = page_scale_factor_ * page_scale_delta;
  SetPageScaleFromImplSide(page_scale);
}
