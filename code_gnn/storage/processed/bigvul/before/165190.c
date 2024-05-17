bool DragController::DragIsMove(FrameSelection& selection,
                                DragData* drag_data) {
  return document_under_mouse_ == drag_initiator_ &&
         selection.SelectionHasFocus() &&
         selection.ComputeVisibleSelectionInDOMTreeDeprecated()
             .IsContentEditable() &&
         selection.ComputeVisibleSelectionInDOMTreeDeprecated().IsRange() &&
         !IsCopyKeyDown(drag_data);
}
