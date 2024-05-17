bool Document::NeedsFullLayoutTreeUpdate() const {
  if (!IsActive() || !View())
    return false;
  if (style_engine_->NeedsActiveStyleUpdate())
    return true;
  if (style_engine_->NeedsWhitespaceReattachment())
    return true;
  if (!use_elements_needing_update_.IsEmpty())
    return true;
  if (NeedsStyleRecalc())
    return true;
  if (NeedsStyleInvalidation())
    return true;
  if (IsSlotAssignmentOrLegacyDistributionDirty())
    return true;
  if (DocumentAnimations::NeedsAnimationTimingUpdate(*this))
    return true;
  return false;
}
