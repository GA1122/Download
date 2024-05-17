const PaintArtifact& PaintController::GetPaintArtifact() const {
  DCHECK(new_display_item_list_.IsEmpty());
  DCHECK(new_paint_chunks_.IsInInitialState());
  return current_paint_artifact_;
}
