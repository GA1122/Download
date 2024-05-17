void PaintController::CheckUnderInvalidation() {
  DCHECK(RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled());

  if (!IsCheckingUnderInvalidation())
    return;

  const DisplayItem& new_item = new_display_item_list_.Last();
  if (new_item.SkippedCache()) {
    under_invalidation_checking_end_ = 0;
    next_item_to_match_ = next_item_to_index_ =
        under_invalidation_checking_begin_;
    return;
  }

  size_t old_item_index = under_invalidation_checking_begin_ +
                          skipped_probable_under_invalidation_count_;
  DisplayItem* old_item =
      old_item_index < current_paint_artifact_.GetDisplayItemList().size()
          ? &current_paint_artifact_.GetDisplayItemList()[old_item_index]
          : nullptr;

  bool old_and_new_equal = (old_item && new_item.Equals(*old_item));
  if (!old_and_new_equal) {
    if (new_item.IsBegin()) {
      ++skipped_probable_under_invalidation_count_;
      return;
    }
    if (new_item.IsDrawing() &&
        skipped_probable_under_invalidation_count_ == 1) {
      DCHECK_GE(new_display_item_list_.size(), 2u);
      if (new_display_item_list_[new_display_item_list_.size() - 2].GetType() ==
          DisplayItem::kBeginCompositing) {
        ++skipped_probable_under_invalidation_count_;
        return;
      }
    }
  }

  if (skipped_probable_under_invalidation_count_ || !old_and_new_equal) {
    ShowUnderInvalidationError(
        "under-invalidation: display item changed",
        new_display_item_list_[new_display_item_list_.size() -
                               skipped_probable_under_invalidation_count_ - 1],
        &current_paint_artifact_
             .GetDisplayItemList()[under_invalidation_checking_begin_]);
    CHECK(false);
  }

  new_display_item_list_.RemoveLast();
  MoveItemFromCurrentListToNewList(old_item_index);

  ++under_invalidation_checking_begin_;
}
