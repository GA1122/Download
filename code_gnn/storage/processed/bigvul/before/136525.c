void PaintController::FinishCycle() {
  DCHECK(new_display_item_list_.IsEmpty());
  DCHECK(new_paint_chunks_.IsInInitialState());

  current_paint_artifact_.FinishCycle();
}
