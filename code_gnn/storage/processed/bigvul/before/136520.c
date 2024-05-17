void PaintController::EndSubsequence(const DisplayItemClient& client,
                                     size_t start) {
  size_t end = new_display_item_list_.size();

  if (RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled() &&
      IsCheckingUnderInvalidation()) {
    SubsequenceMarkers* markers = GetSubsequenceMarkers(client);
    if (!markers && start != end) {
      ShowSequenceUnderInvalidationError(
          "under-invalidation : unexpected subsequence", client, start, end);
      CHECK(false);
    }
    if (markers && markers->end - markers->start != end - start) {
      ShowSequenceUnderInvalidationError(
          "under-invalidation: new subsequence wrong length", client, start,
          end);
      CHECK(false);
    }
  }

  if (start == end) {
    return;
  }

  new_paint_chunks_.ForceNewChunk();

  DCHECK(!new_cached_subsequences_.Contains(&client))
      << "Multiple subsequences for client: " << client.DebugName();

  new_cached_subsequences_.insert(&client, SubsequenceMarkers(start, end));
  last_cached_subsequence_end_ = end;
}
