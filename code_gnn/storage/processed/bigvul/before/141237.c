void Document::SetResizedForViewportUnits() {
  if (media_query_matcher_)
    media_query_matcher_->ViewportChanged();
  if (!HasViewportUnits())
    return;
  EnsureStyleResolver().SetResizedForViewportUnits();
  SetNeedsStyleRecalcForViewportUnits();
}
