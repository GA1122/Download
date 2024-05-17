bool PaintController::LastDisplayItemIsSubsequenceEnd() const {
  return !new_cached_subsequences_.IsEmpty() &&
         last_cached_subsequence_end_ == new_display_item_list_.size();
}
