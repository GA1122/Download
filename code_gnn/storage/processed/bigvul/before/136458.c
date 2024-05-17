void ConversionContext::TranslateForLayerOffsetOnce() {
  if (translated_for_layer_offset_ || layer_offset_.IsZero())
    return;

  cc_list_.StartPaint();
  cc_list_.push<cc::SaveOp>();
  cc_list_.push<cc::TranslateOp>(-layer_offset_.x(), -layer_offset_.y());
  cc_list_.EndPaintOfPairedBegin();
  translated_for_layer_offset_ = true;
}
