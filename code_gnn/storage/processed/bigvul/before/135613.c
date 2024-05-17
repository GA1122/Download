IntRect FrameSelection::AbsoluteCaretBounds() {
  DCHECK(ComputeVisibleSelectionInDOMTree().IsValidFor(*frame_->GetDocument()));
  return frame_caret_->AbsoluteCaretBounds();
}
