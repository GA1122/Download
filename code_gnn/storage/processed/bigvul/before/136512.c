bool PaintController::CacheIsAllInvalid() const {
  DCHECK(!RuntimeEnabledFeatures::SlimmingPaintV2Enabled());
  return current_paint_artifact_.IsEmpty() &&
         current_cache_generation_.GetPaintInvalidationReason() !=
             PaintInvalidationReason::kNone;
}
