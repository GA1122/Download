bool PaintController::UseCachedSubsequenceIfPossible(
    const DisplayItemClient& client) {
  if (DisplayItemConstructionIsDisabled() || SubsequenceCachingIsDisabled())
    return false;

  if (!ClientCacheIsValid(client))
    return false;

  if (RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled() &&
      IsCheckingUnderInvalidation()) {
    return false;
  }

  SubsequenceMarkers* markers = GetSubsequenceMarkers(client);
  if (!markers) {
    return false;
  }

  if (current_paint_artifact_.GetDisplayItemList()[markers->start]
          .IsTombstone()) {
    NOTREACHED();
    return false;
  }

  EnsureNewDisplayItemListInitialCapacity();

  if (next_item_to_match_ == markers->start) {
    next_item_to_match_ = markers->end;
    if (next_item_to_match_ > next_item_to_index_)
      next_item_to_index_ = next_item_to_match_;
  }

  num_cached_new_items_ += markers->end - markers->start;

  if (RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled()) {
    DCHECK(!IsCheckingUnderInvalidation());
    under_invalidation_checking_begin_ = markers->start;
    under_invalidation_checking_end_ = markers->end;
    under_invalidation_message_prefix_ =
        "(In cached subsequence for " + client.DebugName() + ")";
    return false;
  }

  size_t start = BeginSubsequence();
  CopyCachedSubsequence(markers->start, markers->end);
  EndSubsequence(client, start);
  return true;
}
