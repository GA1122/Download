bool WebLocalFrameImpl::IsSelectionAnchorFirst() const {
  FrameSelection& selection = frame_->Selection();
  if (!selection.IsAvailable()) {
    return false;
  }

  return selection.GetSelectionInDOMTree().IsBaseFirst();
}
