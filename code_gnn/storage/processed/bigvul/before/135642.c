void FrameSelection::InvalidatePaint(const LayoutBlock& block,
                                     const PaintInvalidatorContext& context) {
  frame_caret_->InvalidatePaint(block, context);
}
