void PaintController::RemoveLastDisplayItem() {
  DCHECK(!RuntimeEnabledFeatures::SlimmingPaintV175Enabled());

  if (new_display_item_list_.IsEmpty())
    return;

#if DCHECK_IS_ON()
  IndicesByClientMap::iterator it = new_display_item_indices_by_client_.find(
      &new_display_item_list_.Last().Client());
  if (it != new_display_item_indices_by_client_.end()) {
    Vector<size_t>& indices = it->value;
    if (!indices.IsEmpty() &&
        indices.back() == (new_display_item_list_.size() - 1))
      indices.pop_back();
  }
#endif

  if (RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled() &&
      IsCheckingUnderInvalidation()) {
    if (skipped_probable_under_invalidation_count_) {
      --skipped_probable_under_invalidation_count_;
    } else {
      DCHECK(under_invalidation_checking_begin_);
      --under_invalidation_checking_begin_;
      current_paint_artifact_.GetDisplayItemList().RestoreTombstone(
          under_invalidation_checking_begin_, new_display_item_list_.Last());
    }
  }
  new_display_item_list_.RemoveLast();
}
