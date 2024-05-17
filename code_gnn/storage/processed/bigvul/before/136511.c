size_t PaintController::BeginSubsequence() {
  new_paint_chunks_.ForceNewChunk();
  return new_display_item_list_.size();
}
