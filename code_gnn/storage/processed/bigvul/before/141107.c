bool Document::HasPendingForcedStyleRecalc() const {
  return HasPendingVisualUpdate() && !InStyleRecalc() &&
         GetStyleChangeType() >= kSubtreeStyleChange;
}
