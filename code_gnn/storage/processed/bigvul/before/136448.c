void ConversionContext::EndTransform() {
  if (!previous_transform_)
    return;

  cc_list_.StartPaint();
  cc_list_.push<cc::RestoreOp>();
  cc_list_.EndPaintOfPairedEnd();
  current_transform_ = previous_transform_;
  previous_transform_ = nullptr;
}
