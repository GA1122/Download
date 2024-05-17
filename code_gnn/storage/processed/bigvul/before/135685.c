bool FrameSelection::ShouldPaintCaret(const LayoutBlock& block) const {
  DCHECK_GE(GetDocument().Lifecycle().GetState(),
            DocumentLifecycle::kLayoutClean);
  bool result = frame_caret_->ShouldPaintCaret(block);
  DCHECK(!result ||
         (ComputeVisibleSelectionInDOMTree().IsCaret() &&
          IsEditablePosition(ComputeVisibleSelectionInDOMTree().Start())));
  return result;
}
