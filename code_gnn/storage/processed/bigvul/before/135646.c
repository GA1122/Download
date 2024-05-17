void FrameSelection::LayoutBlockWillBeDestroyed(const LayoutBlock& block) {
  frame_caret_->LayoutBlockWillBeDestroyed(block);
}
