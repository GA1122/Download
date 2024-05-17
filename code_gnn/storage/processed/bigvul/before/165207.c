static bool SetSelectionToDragCaret(LocalFrame* frame,
                                    VisibleSelection& drag_caret,
                                    Range*& range,
                                    const LayoutPoint& point) {
  frame->Selection().SetSelectionAndEndTyping(drag_caret.AsSelection());
  if (frame->Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .IsNone()) {
    frame->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
    const PositionWithAffinity& position = frame->PositionForPoint(point);
    if (!position.IsConnected())
      return false;

    frame->Selection().SetSelectionAndEndTyping(
        SelectionInDOMTree::Builder().Collapse(position).Build());
    drag_caret =
        frame->Selection().ComputeVisibleSelectionInDOMTreeDeprecated();
    range = CreateRange(drag_caret.ToNormalizedEphemeralRange());
  }
  return !frame->Selection()
              .ComputeVisibleSelectionInDOMTreeDeprecated()
              .IsNone() &&
         frame->Selection()
             .ComputeVisibleSelectionInDOMTreeDeprecated()
             .IsContentEditable();
}
