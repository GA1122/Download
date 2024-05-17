bool FrameSelection::ComputeAbsoluteBounds(IntRect& anchor,
                                           IntRect& focus) const {
  if (!IsAvailable() || GetSelectionInDOMTree().IsNone())
    return false;

  frame_->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  if (ComputeVisibleSelectionInDOMTree().IsNone()) {
    return false;
  }

  DocumentLifecycle::DisallowTransitionScope disallow_transition(
      frame_->GetDocument()->Lifecycle());

  if (ComputeVisibleSelectionInDOMTree().IsCaret()) {
    anchor = focus = AbsoluteCaretBounds();
  } else {
    const EphemeralRange selected_range =
        ComputeVisibleSelectionInDOMTree().ToNormalizedEphemeralRange();
    if (selected_range.IsNull())
      return false;
    anchor = FirstRectForRange(EphemeralRange(selected_range.StartPosition()));
    focus = FirstRectForRange(EphemeralRange(selected_range.EndPosition()));
  }

  if (!ComputeVisibleSelectionInDOMTree().IsBaseFirst())
    std::swap(anchor, focus);
  return true;
}
