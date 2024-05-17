void SelectionController::PassMousePressEventToSubframe(
    const MouseEventWithHitTestResults& mev) {
  frame_->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  IntPoint p = frame_->View()->RootFrameToContents(
      FlooredIntPoint(mev.Event().PositionInRootFrame()));
  if (!Selection().Contains(p))
    return;

  const VisiblePositionInFlatTree& visible_pos =
      VisiblePositionOfHitTestResult(mev.GetHitTestResult());
  if (visible_pos.IsNull()) {
    Selection().SetSelection(SelectionInDOMTree());
    return;
  }
  Selection().SetSelection(ConvertToSelectionInDOMTree(
      SelectionInFlatTree::Builder()
          .Collapse(visible_pos.ToPositionWithAffinity())
          .Build()));
}
